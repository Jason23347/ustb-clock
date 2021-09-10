#include "conf.h"

#include "stdio.h"
#include "tui.h"
#include <unistd.h>

int
tui_init(tui_t *tui) {
    tui->info.curr_flow = 0;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &tui->winsize);

    if (get_info(&tui->info) == -1) {
        return -1;
    }

    return 0;
}

void
tui_redraw(tui_t *tui) {
#define CLOCK_BLOCK_WIDTH (2 + 3 * CLOCK_DIDGET_WIDTH)
#define CLOCK_DOTS_OFFSET (CLOCK_SPACE_WIDTH * 2 / 3)
/**
 * Calculate position
 * Space between dots and digits is thinner, so minus 2 * offset
 */
#define CLOCK_MIN_WIDTH                                                        \
    5 * CLOCK_BLOCK_WIDTH + 4 * CLOCK_SPACE_WIDTH - 2 * CLOCK_DOTS_OFFSET
/* height: 10 (clock) + 2 (date) + 4 (flow info) */
#define CLOCK_MIN_HEIGHT 16

    // TODO check for minheight and minwidth

    int padding_x = (tui->winsize.ws_col - CLOCK_MIN_WIDTH) / 2,
        padding_y = (tui->winsize.ws_row - CLOCK_MIN_HEIGHT) / 2;

    struct offset clock_offset =
                      {
                          .left = padding_x,
                          .top = padding_y,
                      },
                  info_offset = {
                      .left = (tui->winsize.ws_col - CLOCK_INFO_WIDTH) / 2,
                      .top = padding_y + 12, /* height of digits is 12 */
                  };
    clock_redraw(&tui->clock, clock_offset);
    info_redraw(&tui->info, info_offset);
}
