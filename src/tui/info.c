#include "info.h"

#include "calc.h"
#include "color.h"
#include "conf.h"
#include "draw.h"
#include "net/http.h"
#include <string.h>

offset_t info_offset;

int
info_init(info_t *info) {
    memset(info, 0, sizeof(info_t));

    /* 初始化流量表之前需要获取当前流量信息 */
    if (info_fetch(info) == -1) {
        /* 如果失败大概是必然，先连好网络再说吧 */
        printf("Failed to get flow info.\n");
        return -1;
    }

    /**
     * 初始化用于下载速度计算的流量表，
     * 简而言之就是把第一次 info_fetch得到的数据复制到每一个元素
     */
    flow_t *current = info->flow_arr;
    memcpy(current + 1, current, FLOW_NUM - 1);

    return 0;
}

/* 字符串查找，C语言么有哇 */
char *
strmatch(char *str, const char *pattern, size_t size) {
    char *p = str;
    do {
        p = strchr(p + 1, pattern[0]);
        if (p == 0) {
            return 0;
        }
    } while (strncmp(p, pattern, size - 1) != 0);

    return p;
}

/* 简约的写法，应当仅仅用于静态变量 */
#define strpos(str, pattern) strmatch(str, pattern, sizeof(pattern))
/* 根据fmt读取pattern首次匹配str后面的内容到prop中 */
#define strscan(str, pattern, fmt, prop)                                       \
    {                                                                          \
        str = strpos(p, pattern);                                              \
        if (!str) {                                                            \
            return -1;                                                         \
        }                                                                      \
        sscanf(str + sizeof(pattern) - 1, fmt, &prop);                         \
    }

/* 简单封装读取当前流量的操作，以后会弃用 */
inline void
flow_record(flow_t *flow, unsigned long down_flow) {
    flow->download = down_flow;
    gettimeofday(&flow->tval, 0);
}

/**
 * 读书人的事怎么能说是爬虫呢.
 * Returns 0 if succeed, -1 if not.
 */
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

    /* fee */
    strscan(str, "fee='", "%u", info->fee);

    return 0;
}

void info_setpos(int x, int y) {
    setpos(info_offset, x, y);
}

/* 重绘 info */
void
info_redraw(info_t *info) {
    int err;
    calc_t calc_arr[1], *calc = &calc_arr[0];
    int cur = info->curr_flow;
    offset_t offset = info_offset;

    err = draw_timedlock();
    if (err) {
        debug("%s: Lock error: %s", __FUNCTION__, strerror(err));
        return;
    }

    /* Download speed */
    draw_line(offset,
              "Download:", calc_speed(calc, flow_speed(info->flow_arr, cur)),
              CLOCK_INFO_WIDTH);
    next_line(offset);
    /* IPV6 */
    draw_line(offset, "IPV6 Mode:", calc_ipv6(calc, info->ipv6_mode),
              CLOCK_INFO_WIDTH);
    next_line(offset);
    /* IPV4 Flow */
    if (--cur < 0)
        cur = FLOW_NUM - 1;
    draw_line(offset,
              "IPV4 Flow:", calc_flow(calc, info->flow_arr[cur].download),
              CLOCK_INFO_WIDTH);
    next_line(offset);
    /* Fee Left */
    draw_line(offset, "Fee Left:", calc_fee(calc, info->fee), CLOCK_INFO_WIDTH);
    next_line(offset);

    hidecursor();
    gotoxy(0, 0);

    fflush(stdout);

    draw_unlock();
}
