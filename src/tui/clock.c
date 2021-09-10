#include "conf.h"

#include "clock.h"

int
clock_init(clock_t *clock) {
    clock->color = CLOCK_COLOR;
    clock->digit_width = CLOCK_DIDGET_WIDTH;
    clock->space_width = CLOCK_SPACE_WIDTH;

    return 0;
}

void
clock_redraw(clock_t *clock, struct offset offset) {
    
}
