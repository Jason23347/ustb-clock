#include "flow.h"

#define round(n) (int)(n + 0.5)

unsigned long
flow_wight(unsigned long sec) {
    if (sec == 0)
        return 100;
    else if (sec < 6000000) {
        return round(2.36e-12 * sec * (sec - 12e6) + 100);
    } else {
        return 0;
    }
}

unsigned long
flow_speed(flow_t arr[FLOW_NUM], int current_flow) {
    flow_t *cur, *flow, *last;
    struct timeval *tv;
    unsigned long sec;
    unsigned long weight = 0;
    double res = 0;

    if (--current_flow < 0) {
        current_flow = FLOW_NUM - 1;
    }
    cur = arr + current_flow;

    if (--current_flow < 0) {
        current_flow = FLOW_NUM - 1;
    }
    last = arr + current_flow;

    tv = &cur->tval;

    sec = (tv->tv_sec - last->tval.tv_sec) * 1000000 +
          (tv->tv_usec - last->tval.tv_usec);
    cur->speed = (cur->download - last->download) * 1.0 / (sec + 1);

    for (flow = arr; flow - arr < FLOW_NUM; flow++) {
        sec = (tv->tv_sec - flow->tval.tv_sec) * 1000000 +
              (tv->tv_usec - flow->tval.tv_usec);

        int w = flow_wight(sec);
        weight += w;
        res += w * flow->speed;
    }

    return round(cur->speed * 1000000.0);
}

void
flow_record(flow_t *flow, unsigned long down_flow) {
    flow->download = down_flow;
    gettimeofday(&flow->tval, 0);
}
