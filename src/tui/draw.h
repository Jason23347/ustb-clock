#ifndef DRAW_H
#define DRAW_H

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

void draw_line(offset_t offset, const char *prompt, const calc_t *value,
               int length);
void draw_digit(offset_t offset, int digit);

int draw_lock_init();
int draw_lock();
int draw_timedlock();
int draw_unlock();

#endif /* DRAW_H */