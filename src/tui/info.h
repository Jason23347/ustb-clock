#ifndef INFO_H
#define INFO_H

#include "flow.h"
#include "offset.h"

typedef struct {
    int curr_flow;
    int ipv6_mode;
    unsigned fee;
    flow_t flow_arr[FLOW_NUM];
} info_t;

void info_init(info_t *info);
void info_init_flow(info_t *info);
int get_info(info_t *info);
void info_redraw(info_t *info, offset_t offset);

#endif /* INFO_H */