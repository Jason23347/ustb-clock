#ifndef CLOCK_H
#define CLOCK_H

#define CLOCK_BLOCK_WIDTH (2 + 3 * CLOCK_DOT_WIDTH)
#define CLOCK_DOTS_OFFSET (CLOCK_SPACE_WIDTH * 2 / 3)

/* Space between dots and digits is thinner, so minus 2 * offset */
#define CLOCK_MIN_WIDTH                                                        \
    (5 * CLOCK_BLOCK_WIDTH + 4 * CLOCK_SPACE_WIDTH - 2 * CLOCK_DOTS_OFFSET)

#if (CLOCK_CONDENSE == 0)

/* 10 (clock) + 2 (date) + 4 (flow info) */
#define CLOCK_DIGIT_HEIGHT 10
#define CLOCK_MIN_HEIGHT   16

#else /* CLOCK_CONDENSE */

/*  6 (clock) + 2 (date) + 4 (flow info) */
#define CLOCK_DIGIT_HEIGHT 6
#define CLOCK_MIN_HEIGHT   12

#endif /* CLOCK_CONDENSE */

#include "offset.h"

#include <sys/time.h>

#if (CLOCK_CONDENSE == 0)
#define CLOCK_DIGIT_WIDTH (3 * CLOCK_DOT_WIDTH + 2) // 三个点之间的俩空格
#else
#define CLOCK_DIGIT_WIDTH (3 * CLOCK_DOT_WIDTH)
#endif

typedef struct {
    time_t current;
    struct timeval *tval;
    struct timeval tval_arr[1];
} digits_t;

int clock_init(digits_t *clock);
void clock_update(digits_t *clock, struct timeval *new_time,
                  offset_t offset);
void clock_redraw(digits_t *clock, struct timeval *new_time,
                  offset_t offset);
void clock_draw_digit(int digit);

const char *current_date(char *str);

#endif /* CLOCK_H */