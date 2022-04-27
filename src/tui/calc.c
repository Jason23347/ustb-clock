#include "calc.h"

#include <stdio.h>
#include <string.h>

#include "flow.h"

/* 绝对值 */
#define abs(n) ((n > 0) ? n : -n)

/* 手动添加小数点，保留两位小数 */
const char *
__calc_decimal(char *str, size_t maxlen, int64_t number, size_t n) {
    char *s;
    size_t len;

    // 先处理负号
    s = str;
    if (number < 0) {
        *s = '-';
        s++;
        maxlen--;
        number = -number;
    }

    // 处理小数点
    snprintf(s, maxlen,
#if __WORDSIZE == 64
             "%03ld",
#else
             "%03lld",
#endif
             number);

    len = strlen(s);
    // 小数点后两位向右平移
    s += len - n;
    strncpy(s + 1, s, 2);
    s[0] = '.';
    s[3] = '\0';

    return str;
}

#ifdef COLORFUL_OUTPUT
inline void
__calc_set_color(calc_t *calc, int color) {
    calc->color = color;
}

/* 根据 fee 设置颜色 */
inline int
__color_fee(unsigned fee) {
    if (fee >= 20 * YUAN) {
        return GREEN;
    } else if (fee >= 10 * YUAN) {
        return YELLOW;
    } else {
        return RED;
    }
}
#else
#define __calc_set_color(...)
#endif /* COLORFUL_OUTPUT */

/* 计算余额 */
const calc_t *
calc_fee(calc_t *calc, unsigned fee) {
    __calc_decimal(calc->str, sizeof(calc->str), fee, 4);
    __calc_set_color(calc, __color_fee(fee));

    return calc;
}

/* 计算流量 */
const calc_t *
calc_flow(calc_t *calc, u_int64_t flow) {
    if (flow < 1000) {
        snprintf(calc->str, sizeof(calc->str), u_int64_spec " KB", flow);

        __calc_set_color(calc, NORMAL);
        return calc;
    }

    /* gbflow = 100 代表 1G */
    /* 前100G 免费，所以显示剩余免费额度或已付费流量 */
    u_int64_t gbflow = round(u_int64_t, (float)flow * 100 / MB) - 10000;
    if (abs(flow) < 100) {
        __calc_decimal(calc->str, sizeof(calc->str),
                       round(u_int64_t, (float)flow / KB), 2);
        strncat(calc->str, " MB", sizeof(calc->str));
        __calc_set_color(calc, NORMAL);
    } else {
        __calc_decimal(calc->str, sizeof(calc->str), gbflow, 2);
        strncat(calc->str, " GB", sizeof(calc->str));
        __calc_set_color(calc, NORMAL);
    }

    return calc;
}

#ifdef COLORFUL_OUTPUT
void
__calc_color_flow(calc_t *calc, __uint64_t flow) {
    if (flow <= 1 * KB) {
        calc->color = NORMAL;
    } else if (flow <= 6 * KB) {
        calc->color = YELLOW;
    } else {
        calc->color = RED;
    }
}
#else
#define __calc_color_flow(...)
#endif /* COLORFUL_OUTPUT */

/* 计算流量下载速度 */
const calc_t *
calc_speed(calc_t *calc, __uint64_t flow) {
    if (flow < 1000) {
        snprintf(calc->str, sizeof(calc->str), u_int64_spec " KB/s", flow);
    } else {
        snprintf(calc->str, sizeof(calc->str), "%.2lf MB/s", (float)flow / KB);
    }

    __calc_color_flow(calc, flow);

    return calc;
}

/* 计算 IPV6 模式是否打开 */
const calc_t *
calc_ipv6(calc_t *calc, int mode) {
    if (mode == 4 || mode == 12) {
        strncpy(calc->str, "ON", sizeof(calc->str));
        __calc_set_color(calc, GREEN);
    } else {
        strncpy(calc->str, "OFF", sizeof(calc->str));
        __calc_set_color(calc, YELLOW);
    }

    return calc;
}