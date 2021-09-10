#ifndef DRAW_H
#define DRAW_H

#include "offset.h"

#define gotoxy(x, y)    printf("\033[%d;%dH", (y), (x))
#define goup(y)         printf("\033[%dA", (y))
#define godown(y)       printf("\033[%dB", (y))
#define goright(x)      printf("\033[%dC", (x));
#define goleft(x)       printf("\033[%dD", (x))
#define clear()         printf("\033[2J");

void draw_line(struct offset offset, const char *prompt, const char *value, int length);

#endif /* DRAW_H */