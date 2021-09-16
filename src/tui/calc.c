#include "calc.h"

#include "color.h"
#include <stdio.h>
#include <string.h>

#define round(n) (unsigned long)(n + 0.5)

inline int
color_fee(unsigned fee) {
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
__calc_decimal(char *str, size_t maxlen, unsigned long number, size_t n) {
    size_t len;
    int tmp;

    snprintf(str, maxlen, "%lu", number);
    len = strlen(str);
    tmp = len - n;
    if (tmp < 1) {
        strncpy(str + 1 - tmp, str, len);
        memset(str, '0', 1 - tmp);
        tmp = 3 - n;
    }
    strncpy(str + tmp + 1, str + tmp, 2);
    str[tmp] = '.';
    str[tmp + 3] = '\0';

    return str;
}

calc_t *
calc_fee(calc_t *calc, unsigned fee) {
    __calc_decimal(calc->str, sizeof(calc->str), fee, 4);
    calc->color = color_fee(fee);

    return calc;
}

calc_t *
calc_flow(calc_t *calc, unsigned long flow) {
    if (flow < 1000) {
        snprintf(calc->str, sizeof(calc->str), "%lu KB", flow);
        calc->color = NORMAL;
        return calc;
    }

    /* gbflow = 100 代表 1G */
    unsigned long gbflow = round((float)flow * 100 / 1048576);
    if (gbflow < 100) {
        __calc_decimal(calc->str, sizeof(calc->str), flow, 2);
        strncat(calc->str, " MB", sizeof(calc->str));
        calc->color = NORMAL;
    } else {
        /* 前 50G 免费，所以显示剩余免费额度或已付费流量 */
        gbflow -= 5000;

        __calc_decimal(calc->str, sizeof(calc->str), gbflow, 2);
        strncat(calc->str, " GB", sizeof(calc->str));
        calc->color = NORMAL;
    }

    return calc;
}

calc_t *
calc_speed(calc_t *calc, unsigned long flow) {
    if (flow < 8)
        flow = 0;
    calc_flow(calc, flow);
    strncat(calc->str, "/s", sizeof(calc->str));

    return calc;
}

calc_t *
calc_ipv6(calc_t *calc, int mode) {
    if (mode) {
        strncpy(calc->str, "ON", sizeof(calc->str));
        calc->color = GREEN;
    } else {
        strncpy(calc->str, "OFF", sizeof(calc->str));
        calc->color = YELLOW;
    }

    return calc;
}