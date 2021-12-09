#ifndef RANDMAP_H
#define RANDMAP_H

#include <sys/time.h>

#include "randmap/digitdot.h"

#define CLOCK_DOT_COL      10
#define CLOCK_DOT_ROW      24
#define CLOCK_DIGIT_NUM    (CLOCK_DOT_COL * CLOCK_DOT_ROW)

#define CLOCK_DIGIT_WIDTH  CLOCK_DOT_WIDTH
#define CLOCK_DIGIT_HEIGHT 11
#define CLOCK_MIN_WIDTH    (CLOCK_DIGIT_WIDTH * CLOCK_DOT_ROW)
#define CLOCK_MIN_HEIGHT   (CLOCK_DIGIT_HEIGHT + 6)

typedef struct {
    struct digitdot dots[CLOCK_DIGIT_NUM];
    struct digitdot *cur_dot;
    size_t act_num; /* number of activated dots */
    struct timeval tval;
} digits_t;

int digits_init(digits_t *digits);
int digits_update(digits_t *digits, struct timeval *new_time);
void digits_setpos(int x, int y);
void digits_redraw(digits_t *digits, struct timeval *new_time);

#endif /* RANDMAP_H */