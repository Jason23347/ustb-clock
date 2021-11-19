#include "calc.h"

#include "color.h"
#include <stdio.h>
#include <string.h>

/* 取整 */
#define round(n) (long)(n + 0.5)

/* 绝对值 */
#define abs(n) ((n > 0) ? n : -n)

/* 根据 fee 设置颜色 */
inline int
__color_fee(unsigned fee) {
    if (fee >= 200000) {
        return GREEN;
    } else if (fee >= 100000) {
        return YELLOW;
    } else {
        return RED;
    }
}

/* 手动添加小数点，保留两位小数 */
const char *
__calc_decimal(char *str, size_t maxlen, long number, size_t n) {
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
    snprintf(s, maxlen, "%03ld", number);
    len = strlen(s);
    // 小数点后两位向右平移
    s += len - n;
    strncpy(s + 1, s, 2);
    s[0] = '.';
    s[3] = '\0';

    return str;
}

/* 计算余额 */
calc_t *
calc_fee(calc_t *calc, unsigned fee) {
    __calc_decimal(calc->str, sizeof(calc->str), fee, 4);
    calc->color = __color_fee(fee);

    return calc;
}

/* 计算流量 */
calc_t *
calc_flow(calc_t *calc, unsigned long flow) {
    if (flow < 1000) {
        snprintf(calc->str, sizeof(calc->str), "%lu KB", flow);
        calc->color = NORMAL;
        return calc;
    }

    /* gbflow = 100 代表 1G */
    /* 前 50G 免费，所以显示剩余免费额度或已付费流量 */
    long gbflow = round((float)flow * 100 / 1048576) - 5000;
    if (abs(flow) < 100) {
        __calc_decimal(calc->str, sizeof(calc->str), round((float)flow / 1024),
                       2);
        strncat(calc->str, " MB", sizeof(calc->str));
        calc->color = NORMAL;
    } else {
        __calc_decimal(calc->str, sizeof(calc->str), gbflow, 2);
        strncat(calc->str, " GB", sizeof(calc->str));
        calc->color = NORMAL;
    }

    return calc;
}

/* 计算流量下载速度 */
calc_t *
calc_speed(calc_t *calc, unsigned long flow) {
    if (flow < 1000) {
        snprintf(calc->str, sizeof(calc->str), "%lu KB/s", flow);
    } else {
        /* FIXME 偶尔出现的超大数字，暂时用上界屏蔽了 */
        if (flow > 204800) /* 200 MB/s */
            flow = 0;
        snprintf(calc->str, sizeof(calc->str), "%.2lf MB/s",
                 (float)flow / 1024);
    }

    if (flow <= 1024) {
        calc->color = NORMAL;
    } else if (flow <= 1024 * 6) {
        calc->color = YELLOW;
    } else {
        calc->color = RED;
    }

    return calc;
}

/* 计算 IPV6 模式是否打开 */
calc_t *
calc_ipv6(calc_t *calc, int mode) {
    if (mode == 4 || mode == 12) {
        strncpy(calc->str, "ON", sizeof(calc->str));
        calc->color = GREEN;
    } else {
        strncpy(calc->str, "OFF", sizeof(calc->str));
        calc->color = YELLOW;
    }

    return calc;
}