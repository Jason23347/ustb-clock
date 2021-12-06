#include "conf.h"

#include <time.h>

#include "date.h"

offset_t date_offset;

const char *
date_fmt(char str[CLOCK_DATE_LEN]) {
    time_t arr[1], *cur_time = &arr[0];
    time(cur_time);
    strftime(str, CLOCK_DATE_LEN, CLOCK_DATE_FORMAT, localtime(cur_time));
    return str;
}

void
date_redraw(const char *date_str) {
    gotopos(date_offset);
    printf("%s", date_str);
}

void
date_setpos(int x, int y) {
    setpos(date_offset, x, y);
}