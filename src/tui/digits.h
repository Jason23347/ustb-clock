#ifndef DIGITS_H
#define DIGITS_H

#include <sys/time.h>

#include "draw.h"

#define CLOCK_DOTS_OFFSET (CLOCK_SPACE_WIDTH * 2 / 3)

#if (CLOCK_CONDENSE == 0)
/* 2 是三个点之间的俩空格 */
#define CLOCK_DIGIT_WIDTH  (3 * CLOCK_DOT_WIDTH + 2)
#define CLOCK_DIGIT_HEIGHT 10

#else
#define CLOCK_DIGIT_WIDTH  (3 * CLOCK_DOT_WIDTH)
#define CLOCK_DIGIT_HEIGHT 6

#endif /* CLOCK_CONDENSE */

/* Space between dots and digits is thinner, so minus 2 offsets */
#define CLOCK_MIN_WIDTH                                                        \
    (5 * CLOCK_DIGIT_WIDTH + 4 * CLOCK_SPACE_WIDTH - 2 * CLOCK_DOTS_OFFSET)
/* (clock) + 2 (date) + 4 (flow info) */
#define CLOCK_MIN_HEIGHT (CLOCK_DIGIT_HEIGHT + 6)

typedef struct {
    time_t current;
    struct timeval *tval;
    struct timeval tval_arr[1];
} digits_t;

int digits_init(digits_t *digits);
int digits_update(digits_t *digits, struct timeval *new_time);
void digits_setpos(int x, int y);
void digits_redraw(digits_t *digits, struct timeval *new_time);

#endif /* DIGITS_H */