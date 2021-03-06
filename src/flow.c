#include "conf.h"

#include "flow.h"

/**
 * Calculate weight for time interval,
 * assumed to be never lesser than 0.
 */
inline unsigned
__flow_wight(u_int64_t millisec) {
    if (millisec == 0) {
        return 100;
    } else if (millisec < 6000000) {
        return round(unsigned, 2.36e-12 * millisec * (millisec - 12e6) + 100);
    } else {
        return 15;
    }
}

u_int64_t
flow_speed(flow_t arr[FLOW_NUM], int current_flow) {
    flow_t *cur, *flow, *last;
    struct timeval *tv;
    u_int64_t millisec;
    unsigned weight = 0;
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

    // TODO 提供平滑选项
    // for (flow = arr; flow - arr < FLOW_NUM; flow++) {
    //     millisec = (tv->tv_sec - flow->tval.tv_sec) * 1000000 +
    //                (tv->tv_usec - flow->tval.tv_usec);
    //     unsigned w = __flow_wight(millisec);
    //     weight += w;
    //     res += w * flow->speed;
    // }

    /* FIXME 谜之bug，出现奇大无比的值 */
    {
        u_int64_t tmp = round(u_int64_t, cur->speed * 1000000.0);
        return (tmp > 0 && tmp < 100 * MB) ? tmp : 0;
    }
}
