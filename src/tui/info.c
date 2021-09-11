#include "info.h"

#include "conf.h"
#include "draw.h"
#include "http.h"
#include <string.h>
#include <time.h>

#define MAX_BUFF_SIZE 2048

void
current_date(char *str) {
    time_t time;
    strftime(str, CLOCK_DATE_LEN, CLOCK_DATE_FORMAT, localtime(&time));
}

char *
pattern_match(char *str, const char *pattern, size_t size) {
    char *p = str;
    do {
        p = strchr(p + 1, pattern[0]);
        if (p == 0) {
            return (void *)0;
        }
    } while (strncmp(p, pattern, size - 1) != 0);

    return p;
}

#define strpos(str, pattern) pattern_match(str, pattern, sizeof(pattern))
#define strscan(str, pattern, fmt, prop)                                       \
    str = strpos(p, pattern);                                                  \
    if (!str) {                                                                \
        goto free;                                                             \
    }                                                                          \
    sscanf(str + sizeof(pattern) - 1, fmt, &prop);

int
get_info(info_t *info) {
    http_t http = {
        .ip = LOGIN_HOST,
        .port = PORT,
    };
    buff_t buffs[1], *buff = &buffs[0];

    if (buff_init(buff, MAX_BUFF_SIZE) == -1) {
        return -1;
    }

    if (http_get(&http, buff) == -1) {
        goto free;
    }

    char *str, *p = strpos(buff->str, "<script");
    if (p == 0) {
        debug("failed to get variables: %s\n", buff->str);
        goto free;
    }

    /* flow */
    info->last_flow = info->curr_flow;
    strscan(str, "flow='", "%lu", info->curr_flow);
    if (info->last_flow == 0) {
        info->last_flow = info->curr_flow;
    }

    /* ipv6 mode */
    strscan(str, "v46m=", "%u", info->ipv6_mode);
    info->ipv6_mode = (info->ipv6_mode == 4 || info->ipv6_mode == 12);

    /* fee */
    strscan(str, "fee='", "%u", info->fee);

    buff_free(buff);
    return 0;
free:
    buff_free(buff);
    return -1;
}

void
info_redraw(info_t *info, struct offset offset) {
    /* Download speed */
    draw_line(offset, "Download:", "704 KB/s", CLOCK_INFO_WIDTH);
    next_line(offset);
    /* IPV6 */
    draw_line(offset, "IPV6 Mode:", "ON", CLOCK_INFO_WIDTH);
    next_line(offset);
    /* IPV4 Flow */
    draw_line(offset, "IPV4 Flow:", "56.23 GB", CLOCK_INFO_WIDTH);
    next_line(offset);
    /* Fee Left */
    draw_line(offset, "Fee Left:", "10.00", CLOCK_INFO_WIDTH);
    next_line(offset);
}