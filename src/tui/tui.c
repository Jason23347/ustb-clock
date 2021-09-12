#include "conf.h"

#include "draw.h"
#include "stdio.h"
#include "string.h"
#include "tui.h"
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mux_printf;

int
tui_init(tui_t *tui) {
    pthread_t clock_th, info_th;

    tui->info.curr_flow = 0;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &tui->winsize);

    if (get_info(&tui->info) == -1) {
        return -1;
    }

    clear();
    hidecursor();

    tui_redraw(tui);

    pthread_mutex_init(&mux_printf, 0);
    pthread_create(&clock_th, 0, &clock_schedule, tui);
    pthread_create(&info_th, 0, &info_schedule, tui);

    /* 等了个寂寞 */
    pthread_join(clock_th, 0);

    showcursor();
    return 0;
}

void
tui_redraw(tui_t *tui) {
    // TODO check for minheight and minwidth
    int padding_x = (tui->winsize.ws_col - CLOCK_MIN_WIDTH) / 2,
        padding_y = (tui->winsize.ws_row - CLOCK_MIN_HEIGHT) / 2;

    struct offset offset = {
        .left = padding_x,
        .top = padding_y,
    };

    time_t timer;
    time(&timer);

    struct timeval tval;
    gettimeofday(&tval, 0);

    clock_redraw(&tui->clock, &tval, offset);

    /* draw date */
    char date_str[CLOCK_DATE_LEN];
    current_date(date_str);
    offset.left = (tui->winsize.ws_col - strlen(date_str)) / 2;
    offset.top = padding_y + CLOCK_DIGIT_HEIGHT;

    date_redraw(offset, date_str);

    offset.left = (tui->winsize.ws_col - CLOCK_INFO_WIDTH) / 2;
    offset.top = padding_y + CLOCK_DIGIT_HEIGHT + 2; /* height of date is 2 */
    info_redraw(&tui->info, offset);

    gotoxy(0, 0);
}

void *
clock_schedule(void *arg) {
    tui_t *tui = arg;
    struct timeval tval;

    struct offset offset = {
        .left = (tui->winsize.ws_col - CLOCK_MIN_WIDTH) / 2,
        .top = (tui->winsize.ws_row - CLOCK_MIN_HEIGHT) / 2,
    };

    for (;;) {
        pthread_mutex_lock(&mux_printf);

        gettimeofday(&tval, 0);
        clock_update(&tui->clock, &tval, offset);
        fflush(stdout);

        pthread_mutex_unlock(&mux_printf);

        /* 对齐这一分钟的最后一秒 */
        gettimeofday(&tval, 0);
        struct tm *tmp = localtime(&tval.tv_sec);
        usleep((60 - tmp->tm_sec) * 1000000 - tval.tv_usec);
    }

    return ((void *)0);
}

void *
info_schedule(void *arg) {
    tui_t *tui = arg;
    struct timeval tval;

    struct offset offset = {
        .left = (tui->winsize.ws_col - CLOCK_INFO_WIDTH) / 2,
        .top = (tui->winsize.ws_row - CLOCK_MIN_HEIGHT) / 2 +
               CLOCK_DIGIT_HEIGHT + 2,
    };

    for (;;) {
        pthread_mutex_lock(&mux_printf);

        get_info(&tui->info);
        info_redraw(&tui->info, offset);
        fflush(stdout);

        pthread_mutex_unlock(&mux_printf);

        /* update every second */
        gettimeofday(&tval, 0);
        usleep(1000000 - tval.tv_usec);
    }

    return ((void *)0);
}
