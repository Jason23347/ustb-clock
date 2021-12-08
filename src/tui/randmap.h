#ifndef RANDMAP_H
#define RANDMAP_H

#include <sys/time.h>

#include "randmap/digitdot.h"

#define CLOCK_DIGIT_WIDTH  2
#define CLOCK_DIGIT_HEIGHT 10
#define CLOCK_DIGIT_NUM    (CLOCK_MIN_WIDTH * CLOCK_DIGIT_HEIGHT)
#define CLOCK_MIN_WIDTH    (CLOCK_DIGIT_WIDTH * 12)
#define CLOCK_MIN_HEIGHT   (CLOCK_DIGIT_HEIGHT + 6)

typedef struct {
    struct digitdot dots[CLOCK_DIGIT_NUM];
} digits_t;

int digits_init(digits_t *digits);
int digits_update(digits_t *digits, struct timeval *new_time);
void digits_setpos(int x, int y);
void digits_redraw(digits_t *digits, struct timeval *new_time);

#endif /* RANDMAP_H */