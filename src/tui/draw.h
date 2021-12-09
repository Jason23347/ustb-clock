#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>

#include "calc.h"
#include "offset.h"

#define hidecursor() printf("\033[?25l")
#define showcursor() printf("\033[?25h")

#define savecursor() printf("\033[s")
#define loadcursor() printf("\033[u")

#define gotoxy(x, y) printf("\033[%d;%dH", (int)(y), (int)(x))
#define goup(y)      printf("\033[%dA", (int)(y))
#define godown(y)    printf("\033[%dB", (int)(y))
#define goright(x)   printf("\033[%dC", (int)(x))
#define goleft(x)    printf("\033[%dD", (int)(x))
#define clear()      printf("\033c")
#define flush()      fflush(stdout)

void draw_line(offset_t offset, const char *prompt, const calc_t *value,
               int length);
#if CLOCK_TYPE == CLOCK_TYPE_DIGITS
void draw_digit(offset_t offset, int digit);
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