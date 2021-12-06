#ifndef DATE_H
#define DATE_H

#include "draw.h"

const char *date_fmt(char str[CLOCK_DATE_LEN]);
void date_redraw(const char *date_str);
void date_setpos(int x, int y);

#endif /* DATE_H */