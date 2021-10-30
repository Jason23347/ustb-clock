#include "flow.h"

#define round(n) (int)(n + 0.5)

/**
 * Calculate weight for time interval,
 * assumed to be never lesser than 0.
 */
inline unsigned
__flow_wight(unsigned long millisec) {
    if (millisec == 0)
        return 100;
    else if (millisec < 6000000) {
        return round(2.36e-12 * millisec * (millisec - 12e6) + 100);
    } else {
        return 15;
    }
}

unsigned long
flow_speed(flow_t arr[FLOW_NUM], int current_flow) {
    flow_t *cur, *flow, *last;
    struct timeval *tv;
    unsigned long millisec;
    unsigned long weight = 0;
    double res = 0;

    /* 分支即优化 */
    if (current_flow == 0) {
        cur = arr + FLOW_NUM - 1;
        last = arr + FLOW_NUM - 2;
    } else if (current_flow == 1) {
        cur = arr;
        last = arr + FLOW_NUM - 1;
    } else {
        cur = arr + current_flow - 1;
        last = cur - 1;
    }

    tv = &cur->tval;

    millisec = (tv->tv_sec - last->tval.tv_sec) * 1000000 +
               (tv->tv_usec - last->tval.tv_usec);
    cur->speed = (cur->download - last->download) * 1.0 / (millisec + 1);

    for (flow = arr; flow - arr < FLOW_NUM; flow++) {
        millisec = (tv->tv_sec - flow->tval.tv_sec) * 1000000 +
                   (tv->tv_usec - flow->tval.tv_usec);

        unsigned w = __flow_wight(millisec);
        weight += w;
        res += w * flow->speed;
    }

    /* FIXME 谜之bug，出现奇大无比的负值 */
    if (cur->speed < 0)
        cur->speed = 0;

    return round(cur->speed * 1000000.0);
}
