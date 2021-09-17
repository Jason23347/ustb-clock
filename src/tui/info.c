#include "info.h"

#include "calc.h"
#include "color.h"
#include "conf.h"
#include "draw.h"
#include "net/http.h"
#include <string.h>

int
info_init(info_t *info) {
    memset(info->flow_arr, 0, sizeof(info->flow_arr));

    if (info_fetch(info) == -1) {
        printf("Failed to get flow info.\n");
        return -1;
    }

    /* 初始化 */
    flow_t *current = info->flow_arr;
    for (flow_t *flow = current + 1; flow - current < FLOW_NUM; flow++) {
        flow->download = current->download;
        flow->tval.tv_sec = current->tval.tv_sec;
        flow->tval.tv_usec = current->tval.tv_usec;
    }

    return 0;
}

char *
strmatch(char *str, const char *pattern, size_t size) {
    char *p = str;
    do {
        p = strchr(p + 1, pattern[0]);
        if (p == 0) {
            return (void *)0;
        }
    } while (strncmp(p, pattern, size - 1) != 0);

    return p;
}

#define strpos(str, pattern) strmatch(str, pattern, sizeof(pattern))
#define strscan(str, pattern, fmt, prop)                                       \
    {                                                                          \
        str = strpos(p, pattern);                                              \
        if (!str) {                                                            \
            return -1;                                                         \
        }                                                                      \
        sscanf(str + sizeof(pattern) - 1, fmt, &prop);                         \
    }

int
info_fetch(info_t *info) {
    unsigned long flow;
    http_t http = {
        .ip = LOGIN_HOST,
        .port = PORT,
    };

    if (http_get(&http) == -1) {
        return -1;
    }

    char *str, *p = strpos(http.buff, "<script");
    if (p == 0) {
        debug("failed to get variables: %s\n", http.buff);
        return -1;
    }

    /* flow */
    strscan(str, "flow='", "%lu", flow);
    flow_record(&info->flow_arr[info->curr_flow], flow);
    if (++info->curr_flow >= FLOW_NUM) {
        info->curr_flow -= FLOW_NUM;
    }

    /* ipv6 mode */
    strscan(str, "v46m=", "%u", info->ipv6_mode);
    info->ipv6_mode = (info->ipv6_mode == 4 || info->ipv6_mode == 12);

    /* fee */
    strscan(str, "fee='", "%u", info->fee);

    return 0;
}

void
info_redraw(info_t *info, offset_t offset) {
    calc_t calc_arr[1], *calc = &calc_arr[0];
    /* Download speed */
    draw_line(offset, "Download:",
              calc_speed(calc, flow_speed(info->flow_arr, info->curr_flow)),
              CLOCK_INFO_WIDTH);
    next_line(offset);
    /* IPV6 */
    draw_line(offset, "IPV6 Mode:", calc_ipv6(calc, info->ipv6_mode),
              CLOCK_INFO_WIDTH);
    next_line(offset);
    /* IPV4 Flow */
    draw_line(offset, "IPV4 Flow:",
              calc_flow(calc, info->flow_arr[info->curr_flow].download),
              CLOCK_INFO_WIDTH);
    next_line(offset);
    /* Fee Left */
    draw_line(offset, "Fee Left:", calc_fee(calc, info->fee), CLOCK_INFO_WIDTH);
    next_line(offset);

    gotoxy(0, 0);
}