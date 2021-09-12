#include "conf.h"

#include "digits.h"
#include "draw.h"

#include <time.h>

const char *
current_date(char str[CLOCK_DATE_LEN]) {
    time_t arr[1], *cur_time = &arr[0];
    time(cur_time);
    strftime(str, CLOCK_DATE_LEN, CLOCK_DATE_FORMAT, localtime(cur_time));
    return str;
}

int
clock_init(digits_t *clock) {
    // Not implemented yet.
    return 0;
}

void
clock_redraw(digits_t *clock, struct offset offset) {
#if (CLOCK_CONDENSE == 0)
#define CLOCK_DIGIT_WIDTH (3 * CLOCK_DOT_WIDTH + 2)
#else
#define CLOCK_DIGIT_WIDTH (3 * CLOCK_DOT_WIDTH)
#endif
    draw_digit(offset, 0);
    transpos(offset, CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH, 0);
    draw_digit(offset, 8);
    transpos(offset, CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH - 2, 0);
    draw_digit(offset, -1);
    transpos(offset, CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH - 2, 0);
    draw_digit(offset, 3);
    transpos(offset, CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH, 0);
    draw_digit(offset, 7);

    setpos(offset, 0, offset.top + CLOCK_DIGIT_HEIGHT);
}
