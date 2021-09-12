#include "info.h"

#include "calc.h"
#include "color.h"
#include "conf.h"
#include "draw.h"
#include "net/http.h"
#include <string.h>
#include <time.h>

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
        return -1;                                                             \
    }                                                                          \
    sscanf(str + sizeof(pattern) - 1, fmt, &prop);

int
get_info(info_t *info) {
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

    return 0;
}

void
info_redraw(info_t *info, struct offset offset) {
    calc_t calcs[1], *calc = &calcs[0];
    /* Download speed */
    draw_line(offset,
              "Download:", calc_speed(calc, info->curr_flow - info->last_flow),
              CLOCK_INFO_WIDTH);
    next_line(offset);
    /* IPV6 */
    draw_line(offset, "IPV6 Mode:", calc_ipv6(calc, info->ipv6_mode),
              CLOCK_INFO_WIDTH);
    next_line(offset);
    /* IPV4 Flow */
    draw_line(offset, "IPV4 Flow:", calc_flow(calc, info->curr_flow),
              CLOCK_INFO_WIDTH);
    next_line(offset);
    /* Fee Left */
    draw_line(offset, "Fee Left:", calc_fee(calc, info->fee), CLOCK_INFO_WIDTH);
    next_line(offset);
}