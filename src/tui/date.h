#ifndef DATE_H
#define DATE_H

const char *date_fmt(char str[CLOCK_DATE_LEN]);
void date_redraw(const char *date_str);
void date_setpos(size_t x, size_t y);

#endif /* DATE_H */