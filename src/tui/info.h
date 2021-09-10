#ifndef INFO_H
#define INFO_H

#include "offset.h"

typedef struct {
    unsigned long last_flow;
    unsigned long curr_flow;
    int ipv6_mode;
    unsigned fee;
} info_t;

const char *download_speed();
const char *ipv6_mode();
const char *ipv4_flow();
const char *fee_left();
void current_date(char *str);

int get_info(info_t *info);
void info_redraw(info_t *info, struct offset offset);

#endif /* INFO_H */