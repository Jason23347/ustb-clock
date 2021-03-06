#ifndef INFO_H
#define INFO_H

#include "flow.h"

typedef struct {
    int curr_flow;
    int ipv6_mode;
    unsigned fee;
    flow_t flow_arr[FLOW_NUM];
} info_t;

int info_init(info_t *info);
int info_fetch(info_t *info);

void info_setpos(size_t x, size_t y);
void info_redraw(info_t *info);
void info_printerr(info_t *info);

#endif /* INFO_H */