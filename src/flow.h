#ifndef FLOW_H
#define FLOW_H

#include <sys/time.h>

#define FLOW_NUM 8

typedef struct {
    struct timeval tval;
    unsigned long download;
    unsigned long speed;
} flow_t;

unsigned long flow_speed(flow_t arr[FLOW_NUM], int current_flow);
void flow_record(flow_t *flow, unsigned long down_flow);

#endif /* FLOW_H */