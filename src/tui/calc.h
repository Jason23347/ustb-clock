#ifndef CALC_H
#define CALC_H

#include "conf.h"
#include <bits/types.h>

#if __WORDSIZE == 64
#define uint64_specifier "%lu"
#else
#define uint64_specifier "%llu"
#endif

#define YUAN 1000

typedef struct {
#ifdef COLORFUL_OUTPUT
    int color;
#endif /* COLORFUL_OUTPUT */
    char str[CLOCK_INFO_WIDTH];
} calc_t;

calc_t *calc_flow(calc_t *calc, __uint64_t flow);
calc_t *calc_speed(calc_t *calc, __uint64_t flow);
calc_t *calc_fee(calc_t *calc, unsigned fee);
calc_t *calc_ipv6(calc_t *calc, int mode);

#endif /* CALC_H */