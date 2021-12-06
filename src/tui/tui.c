#include "conf.h"

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tui.h"
#include "date.h"
#include "draw.h"
#include "sched/watcher.h"

tui_t tui_struct;

offset_t clock_offset, date_offset, info_offset;

pthread_t clock_th, info_th, watcher_th;

watcher_t watcher;

#ifdef ALWAYS_DETECT_WINSIZE

void
tui_restart(int sig) {
    if (sig == SIGALRM) {
        ioctl(STDIN_FILENO, TIOCGWINSZ, &tui_struct.win);

        draw_timedlock();
        tui_redraw(0);
        draw_unlock();

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
    int err;
    char date_str[CLOCK_DATE_LEN];

    err = draw_timedlock();
    if (err) {
        // TODO handle error
    }

    date_fmt(date_str);
    date_redraw(date_offset, date_str);

    fflush(stdout);

    draw_unlock();

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

    clock_init(&tui->clock);
    info_init(&tui->info);

    /* 初始化watcher */
    watcher_init(&watcher);

    /* Clear screen hourly */
    task_init(&task, HOURLY, tui_redraw);
    watcher_register(&watcher, task);
    /* Date update */
    task_init(&task, DAILY, date_update);
    watcher_register(&watcher, task);

    tui_redraw(0);

    draw_lock_init();
    pthread_create(&watcher_th, 0, &watcher_schedule, &watcher);
    pthread_create(&clock_th, 0, &clock_schedule, tui);
    pthread_create(&info_th, 0, &info_schedule, tui);

    /* 等了个寂寞 */
    pthread_join(clock_th, 0);

    return 0;
}

/* Compat to task initialization */
void *
tui_redraw(int num) {
    tui_t *tui = &tui_struct;
    struct timeval tval;
    char date_str[CLOCK_DATE_LEN];

    /* Clear screen */
    hidecursor();
    clear();

    // TODO check for minheight and minwidth
    int padding_y = (tui->win.ws_row - CLOCK_MIN_HEIGHT) / 2;
    setpos(clock_offset, (tui->win.ws_col - CLOCK_MIN_WIDTH) / 2, padding_y);

    gettimeofday(&tval, 0);
    clock_redraw(&tui->clock, &tval, clock_offset);

    /* draw date */
    padding_y += CLOCK_DIGIT_HEIGHT;
    date_fmt(date_str);
    setpos(date_offset, (tui->win.ws_col - strlen(date_str)) / 2, padding_y);

    date_redraw(date_offset, date_str);

    setpos(info_offset, (tui->win.ws_col - CLOCK_INFO_WIDTH) / 2,
           padding_y + 2); /* height of date is 2 */
    info_redraw(&tui->info, info_offset);

    gotoxy(0, 0);

    fflush(stdout);

    return 0;
}

void *
clock_schedule(void *arg) {
    tui_t *tui = arg;

    int err;
    struct timeval tval;

    for (;;) {
        err = draw_timedlock();
        if (err) {
            // TODO handle error
            debug("Lock error: %s", strerror(err));
            goto next_tick;
        }

        gettimeofday(&tval, 0);
        watcher.num = clock_update(&tui->clock, &tval, clock_offset);
        fflush(stdout);

        pthread_cond_signal(&watcher.cond);

        draw_unlock();

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
    tui_t *tui = arg;

    int err;
    struct timeval tval;

    for (;;) {
        if (info_fetch(&tui->info) == -1) {
            goto next_tick;
        }

        err = draw_timedlock();
        if (err) {
            // TODO handle error
            goto next_tick;
        }
        info_redraw(&tui->info, info_offset);
        fflush(stdout);

        draw_unlock();

    next_tick:
        /* update every INFO_REFRESH_INTERVAL seconds */
        gettimeofday(&tval, 0);
        usleep(1000000 * INFO_REFRESH_INTERVAL - tval.tv_usec);
    }

    return ((void *)0);
}
