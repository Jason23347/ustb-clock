#include "conf.h"

#include "tui.h"

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "date.h"
#include "digits.h"
#include "info.h"

#include "sched/watcher.h"

tui_t tui_struct;

pthread_t clock_th, info_th, watcher_th;

watcher_t watcher;

#ifdef ALWAYS_DETECT_WINSIZE

void
tui_restart(int sig) {
    if (sig == SIGALRM) {
        ioctl(STDIN_FILENO, TIOCGWINSZ, &tui_struct.win);

        tui_redraw(0);

        struct itimerval timer = {0};
        setitimer(ITIMER_REAL, &timer, 0);
    }
}

inline void
debounce() {
    struct itimerval timer = {
        .it_interval = {.tv_usec = 10000}, // 10ms
        .it_value = {.tv_usec = 10},
    };

    setitimer(ITIMER_REAL, &timer, 0);
}

void
handle_winch(int sig) {
    if (sig == SIGWINCH) {
        debounce();
    }
}

#endif /* ALWAYS_DETECT_WINSIZE */

void
handle_abort(int signal) {
    pthread_cancel(watcher_th);
    pthread_cancel(clock_th);
    pthread_cancel(info_th);

    showcursor();
    clear();

    gotoxy(0, 0);
    printf("Bye-bye.\n");
    fflush(stdout);
}

void *
date_update(int num) {
    char date_str[CLOCK_DATE_LEN];
    date_redraw(date_fmt(date_str));
    return 0;
}

int
tui_init() {
    tui_t *tui = &tui_struct;
    task_t task;

#ifdef ALWAYS_DETECT_WINSIZE
    signal(SIGWINCH, handle_winch);
    signal(SIGALRM, tui_restart);
#endif
    signal(SIGINT, handle_abort);

    ioctl(STDIN_FILENO, TIOCGWINSZ, &tui->win);

    digits_init(&tui->clock);
    info_init(&tui->info);

    /* 初始化watcher */
    watcher_init(&watcher);

    /* Clear screen hourly */
    task_init(&task, HOURLY, tui_redraw);
    watcher_register(&watcher, task);
    /* Date update */
    task_init(&task, DAILY, date_update);
    watcher_register(&watcher, task);

    draw_lock_init();
    tui_redraw(0);

    pthread_create(&watcher_th, 0, &watcher_schedule, &watcher);
    pthread_create(&clock_th, 0, &digits_schedule, &tui->clock);
    pthread_create(&info_th, 0, &info_schedule, &tui->info);

    /* 等了个寂寞 */
    pthread_join(clock_th, 0);

    return 0;
}

/* Compat to task initialization */
void *
tui_redraw(int num) {
    struct timeval tval;
    char date_str[CLOCK_DATE_LEN];
    tui_t *tui = &tui_struct;

    /* Clear screen */
    hidecursor();
    clear();

    // TODO check for minheight and minwidth
    int padding_y = (tui->win.ws_row - CLOCK_MIN_HEIGHT) / 2;
    digits_setpos((tui->win.ws_col - CLOCK_MIN_WIDTH) / 2, padding_y);

    gettimeofday(&tval, 0);
    digits_redraw(&tui->clock, &tval);

    /* draw date */
    padding_y += CLOCK_DIGIT_HEIGHT;
    date_fmt(date_str);
    date_setpos((tui->win.ws_col - strlen(date_str)) / 2, padding_y);
    date_redraw(date_str);

    info_setpos((tui->win.ws_col - CLOCK_INFO_WIDTH) / 2,
                padding_y + 2); /* height of date is 2 */
    info_redraw(&tui->info);

    gotoxy(0, 0);
    fflush(stdout);

    return 0;
}

void *
digits_schedule(void *arg) {
    digits_t *digits = arg;
    struct timeval tval;

    for (;;) {
        gettimeofday(&tval, 0);
        watcher.num = digits_update(digits, &tval);
        if (watcher.num < 0) {
            goto next_tick;
        }

        watcher_awake(&watcher);

    next_tick:
        /* 对齐这一分钟的最后一秒 */
        gettimeofday(&tval, 0);
        struct tm *tmp = localtime(&tval.tv_sec);
        usleep((60 - tmp->tm_sec) * 1000000 - tval.tv_usec);
    }

    return ((void *)0);
}

void *
info_schedule(void *arg) {
    struct timeval tval;
    info_t *info = arg;

    for (;;) {
        if (info_fetch(info) == -1) {
            goto next_tick;
        }

        info_redraw(info);

    next_tick:
        /* update every INFO_REFRESH_INTERVAL seconds */
        gettimeofday(&tval, 0);
        usleep(1000000 * INFO_REFRESH_INTERVAL - tval.tv_usec);
    }

    return ((void *)0);
}
