#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>

#include "calc.h"
#include "offset.h"

#define hidecursor() printf("\033[?25l")
#define showcursor() printf("\033[?25h")

#define savecursor() printf("\033[s")
#define loadcursor() printf("\033[u")

#define gotoxy(x, y)                                                           \
    printf("\033[" ssize_spec ";" ssize_spec "H", (ssize_t)(y), (ssize_t)(x))
#define goup(y)    printf("\033[" ssize_spec "A", (ssize_t)(y))
#define godown(y)  printf("\033[" ssize_spec "B", (ssize_t)(y))
#define goright(x) printf("\033[" ssize_spec "C", (ssize_t)(x))
#define goleft(x)  printf("\033[" ssize_spec "D", (ssize_t)(x))
#define clear()    printf("\033c")
#define flush()    fflush(stdout)

void draw_line(offset_t offset, const char *prompt, const calc_t *value,
               size_t length);

#ifdef COLORFUL_OUTPUT
void draw_center(offset_t offset, const char *str, int color, size_t length);
#else
void draw_center(offset_t offset, const char *str, size_t length);
#endif /* COLORFUL_OUTPUT */
void draw_empty_line(offset_t offset, size_t length);

#if CLOCK_TYPE == CLOCK_TYPE_DIGITS
void draw_digit(offset_t offset, size_t digit);
#elif CLOCK_TYPE == CLOCK_TYPE_RANDMAP
void draw_dot(offset_t offset);
#endif /* CLOCK_TYPE */

int draw_lock_init();
int draw_lock();
int draw_timedlock();
int draw_unlock();

void draw_start();
void draw_end();

#endif /* DRAW_H */