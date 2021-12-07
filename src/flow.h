#ifndef FLOW_H
#define FLOW_H

#include <sys/time.h>

#define FLOW_NUM 8

typedef struct {
    struct timeval tval;
    __uint64_t download;
    double speed;
} flow_t;

__uint64_t flow_speed(flow_t arr[FLOW_NUM], int current_flow);

#endif /* FLOW_H */