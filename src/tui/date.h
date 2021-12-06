#ifndef DATE_H
#define DATE_H

#include "draw.h"

const char *date_fmt(char str[CLOCK_DATE_LEN]);
void date_redraw(offset_t offset, const char *date_str);

#endif /* DATE_H */