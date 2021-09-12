#ifndef DRAW_H
#define DRAW_H

#include "calc.h"
#include "offset.h"

#define hidecursor() printf("\33[?25l")
#define showcursor() printf("\33[?25h")

#define gotoxy(x, y) printf("\033[%d;%dH", (int)(y), (int)(x))
#define goup(y)      printf("\033[%dA", (int)(y))
#define godown(y)    printf("\033[%dB", (int)(y))
#define goright(x)   printf("\033[%dC", (int)(x))
#define goleft(x)    printf("\033[%dD", (int)(x))
#define clear()      printf("\033[2J");

void draw_line(struct offset offset, const char *prompt, const calc_t *value,
               int length);
void draw_digit(struct offset offset, int digit);
void date_redraw(struct offset offset, const char *date_str);

#endif /* DRAW_H */