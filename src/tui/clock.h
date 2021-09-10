#ifndef CLOCK_H
#define CLOCK_H

#include "offset.h"

typedef struct {
    const char *color;
    unsigned digit_width;
    unsigned space_width;
} clock_t;

int clock_init(clock_t *clock);
void clock_redraw(clock_t *clock, struct offset offset);
void clock_draw_digit(int digit);

#endif /* CLOCK_H */