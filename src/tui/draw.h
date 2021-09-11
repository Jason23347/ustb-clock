#ifndef DRAW_H
#define DRAW_H

#include "offset.h"

#define gotoxy(x, y)                printf("\033[%d;%dH", (int)(y), (int)(x))
#define goup(y)                     printf("\033[%dA", (int)(y))
#define godown(y)                   printf("\033[%dB", (int)(y))
#define goright(x)                  printf("\033[%dC", (int)(x))
#define goleft(x)                   printf("\033[%dD", (int)(x))
#define clear()                     printf("\033[2J");

#define gotopos(offset)             gotoxy(offset.left + 1, offset.top + 1)
#define transpos(offset, x, y)      { offset.left += x; offset.top += y; }
#define next_line(offset)           transpos(offset, 0, 1)

void draw_line(struct offset offset, const char *prompt, const char *value, int length);
void draw_digit(struct offset offset, int digit);

#endif /* DRAW_H */