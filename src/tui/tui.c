#include "conf.h"

#include "draw.h"
#include "stdio.h"
#include "string.h"
#include "tui.h"
#include <unistd.h>

int
tui_init(tui_t *tui) {
    tui->info.curr_flow = 0;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &tui->winsize);

    if (get_info(&tui->info) == -1) {
        return -1;
    }

    tui_redraw(tui);

    return 0;
}

void
tui_redraw(tui_t *tui) {
    clear();

    // TODO check for minheight and minwidth
    int padding_x = (tui->winsize.ws_col - CLOCK_MIN_WIDTH) / 2,
        padding_y = (tui->winsize.ws_row - CLOCK_MIN_HEIGHT) / 2;

    struct offset offset = {
        .left = padding_x,
        .top = padding_y,
    };

    clock_redraw(&tui->clock, offset);

    /* draw date */
    char date_str[CLOCK_DATE_LEN];
    current_date(date_str);
    offset.left = (tui->winsize.ws_col - strlen(date_str)) / 2;
    offset.top = padding_y + CLOCK_DIGIT_HEIGHT;

    date_redraw(offset, date_str);

    offset.left = (tui->winsize.ws_col - CLOCK_INFO_WIDTH) / 2;
    offset.top = padding_y + CLOCK_DIGIT_HEIGHT + 2; /* height of date is 2 */
    info_redraw(&tui->info, offset);
}
