#include "conf.h"

#include "clock.h"
#include "draw.h"

int
clock_init(clock_t *clock) {
    // Not implemented yet.
    return 0;
}

void
clock_redraw(clock_t *clock, struct offset offset) {
#if (CLOCK_CONDENSE == 0)
#define CLOCK_DIGIT_WIDTH   (3 * CLOCK_DOT_WIDTH + 2)
#else
#define CLOCK_DIGIT_WIDTH   (3 * CLOCK_DOT_WIDTH)
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
}
