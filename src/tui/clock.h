#ifndef CLOCK_H
#define CLOCK_H

#include "offset.h"
typedef struct {
    // 没想好时间怎么定义
} clock_t;

int clock_init(clock_t *clock);
void clock_redraw(clock_t *clock, struct offset offset);
void clock_draw_digit(int digit);

#endif /* CLOCK_H */