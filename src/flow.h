#ifndef FLOW_H
#define FLOW_H

#include <sys/types.h>
#include <sys/time.h>

#define FLOW_NUM 8

#define round(_type, _num) (_type)((_num) + 0.5)

#define KB 1024
#define MB (KB * KB)

typedef struct {
    struct timeval tval;
    u_int64_t download;
    double speed;
} flow_t;

u_int64_t flow_speed(flow_t arr[FLOW_NUM], int current_flow);

#endif /* FLOW_H */