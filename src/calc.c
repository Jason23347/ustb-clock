#include "calc.h"

#include <stdio.h>
#include <string.h>

const char *
calc_fee(char *str, unsigned fee) {
    sprintf(str, "%d", fee);
    /* 手动添加小数点 */
    size_t i = strlen(str);
    str[i] = str[i - 1];
    i--;
    str[i] = str[i - 1];
    i--;
    str[i] = str[i - 1];
    i--;
    str[i] = str[i - 1];
    i--;
    str[i] = '.';
    str[i + 3] = '\0';

    return str;
}

const char *
calc_flow(char *str, unsigned long flow) {
    if (flow / 1024 < 1) {
        sprintf(str, "%lu KB", flow);
    } else if (flow / 1048576 < 1) {
        sprintf(str, "%.2f MB", ((float)flow / 1024));
    } else {
        sprintf(str, "%.2f GB", ((float)flow / 1048576));
    }

    return str;
}

const char *
calc_speed(char *str, unsigned long flow) {
    calc_flow(str, flow);
    strncat(str, "/s", 3);
    return str;
}
