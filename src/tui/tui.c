#include "conf.h"

#include "draw.h"
#include "stdio.h"
#include "string.h"
#include "tui.h"
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

tui_t tui_struct;

offset_t clock_offset, date_offset, info_offset;

pthread_mutex_t mux_draw;
pthread_t clock_th, info_th;

void
tui_restart(int signal) {
    if (signal == SIGWINCH) {
        // TODO 防抖
        ioctl(STDIN_FILENO, TIOCGWINSZ, &tui_struct.win);
        pthread_mutex_lock(&mux_draw);
        clear();
        tui_redraw();
        pthread_mutex_unlock(&mux_draw);
    }
}

void
handle_abort(int signal) {
    pthread_cancel(clock_th);
    pthread_cancel(info_th);

    showcursor();
    clear();

    gotoxy(0, 0);
    printf("Bye-bye.\n");
}

int
tui_init() {
    tui_t *tui = &tui_struct;

    tui->info.curr_flow = 0;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &tui->win);

    signal(SIGWINCH, tui_restart);
    signal(SIGINT, handle_abort);

    clock_init(&tui->clock);
    info_init(&tui->info);

    if (get_info(&tui->info) == -1) {
        return -1;
    }
    info_init_flow(&tui->info);

    clear();
    hidecursor();

    tui_redraw(tui);

    pthread_mutex_init(&mux_draw, 0);
    pthread_create(&clock_th, 0, &clock_schedule, tui);
    pthread_create(&info_th, 0, &info_schedule, tui);

    /* 等了个寂寞 */
    pthread_join(clock_th, 0);

    showcursor();
    return 0;
}

void
tui_redraw() {
    tui_t *tui = &tui_struct;
    time_t timer;
    struct timeval tval;
    char date_str[CLOCK_DATE_LEN];

    // TODO check for minheight and minwidth
    int padding_y = (tui->win.ws_row - CLOCK_MIN_HEIGHT) / 2;

    setpos(clock_offset, (tui->win.ws_col - CLOCK_MIN_WIDTH) / 2, padding_y);

    time(&timer);

    gettimeofday(&tval, 0);

    clock_redraw(&tui->clock, &tval, clock_offset);

    padding_y += CLOCK_DIGIT_HEIGHT;

    /* draw date */
    current_date(date_str);
    setpos(date_offset, (tui->win.ws_col - strlen(date_str)) / 2, padding_y);

    date_redraw(date_offset, date_str);

    /* height of date is 2 */
    setpos(info_offset, (tui->win.ws_col - CLOCK_INFO_WIDTH) / 2,
           padding_y + 2);
    info_redraw(&tui->info, info_offset);

    gotoxy(0, 0);
}

void *
clock_schedule(void *arg) {
    tui_t *tui = arg;
    struct timeval tval;

    for (;;) {
        pthread_mutex_lock(&mux_draw);

        gettimeofday(&tval, 0);
        clock_update(&tui->clock, &tval, clock_offset);
        fflush(stdout);

        pthread_mutex_unlock(&mux_draw);

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

    for (;;) {
        pthread_mutex_lock(&mux_draw);

        get_info(&tui->info);
        info_redraw(&tui->info, info_offset);
        fflush(stdout);

        pthread_mutex_unlock(&mux_draw);

        /* update every second */
        gettimeofday(&tval, 0);
        usleep(1000000 - tval.tv_usec);
    }

    return ((void *)0);
}
