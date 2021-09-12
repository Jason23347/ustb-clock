#include "calc.h"

#include "color.h"
#include <stdio.h>
#include <string.h>

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

calc_t *
calc_fee(calc_t *calc, unsigned fee) {
    snprintf(calc->str, sizeof(calc->str), "%d", fee);
    /* 手动添加小数点 */
    size_t len = strlen(calc->str);
    strncpy(calc->str + len - 3, calc->str + len - 4, 2);
    calc->str[len - 4] = '.';
    calc->str[len - 1] = '\0';

    calc->color = color_fee(fee);

    return calc;
}

calc_t *
calc_flow(calc_t *calc, unsigned long flow) {
    if (flow / 1024 < 1) {
        snprintf(calc->str, sizeof(calc->str), "%lu KB", flow);
    } else if (flow / 1048576 < 1) {
        snprintf(calc->str, sizeof(calc->str), "%.2f MB", ((float)flow / 1024));
    } else {
        float gbflow = (float)flow / 1048576;
        /* 前 50G 免费，所以显示剩余免费额度 */
        gbflow -= 50;
        snprintf(calc->str, sizeof(calc->str), "%.2f GB", gbflow);
    }

    calc->color = NORMAL;

    return calc;
}

calc_t *
calc_speed(calc_t *calc, unsigned long flow) {
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