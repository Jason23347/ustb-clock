#include "conf.h"

#include "draw.h"
#include "stdio.h"
#include "string.h"
#include "tui.h"
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

tui_t tui_struct;

offset_t clock_offset, date_offset, info_offset;

pthread_mutex_t mux_draw;
pthread_t clock_th, info_th;

#ifdef ALWAYS_DETECT_WINSIZE

void
tui_restart(int sig) {
    if (sig == SIGALRM) {
        struct timespec tspec;
        ioctl(STDIN_FILENO, TIOCGWINSZ, &tui_struct.win);

        clock_gettime(CLOCK_REALTIME, &tspec);
        tspec.tv_sec++;
        pthread_mutex_timedlock(&mux_draw, &tspec);
        clear();
        tui_redraw();
        pthread_mutex_unlock(&mux_draw);

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

#ifdef ALWAYS_DETECT_WINSIZE
    signal(SIGWINCH, handle_winch);
    signal(SIGALRM, tui_restart);
#endif
    signal(SIGINT, handle_abort);

    ioctl(STDIN_FILENO, TIOCGWINSZ, &tui->win);

    clock_init(&tui->clock);
    info_init(&tui->info);

    clear();
    hidecursor();

    tui_redraw();

    pthread_mutex_init(&mux_draw, 0);
    pthread_create(&clock_th, 0, &clock_schedule, tui);
    pthread_create(&info_th, 0, &info_schedule, tui);

    /* 等了个寂寞 */
    pthread_join(clock_th, 0);

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

    int err;
    struct timeval tval;
    struct timespec tspec;
    char date_str[CLOCK_DATE_LEN];

    for (;;) {
        clock_gettime(CLOCK_REALTIME, &tspec);
        tspec.tv_sec++;
        err = pthread_mutex_timedlock(&mux_draw, &tspec);
        if (err) {
            // TODO handle error
            debug("Lock error: %s", strerror(err));
            goto next_tick;
        }

        gettimeofday(&tval, 0);
        int num = clock_update(&tui->clock, &tval, clock_offset);
        if (num > 2) {
            /* draw date */
            current_date(date_str);
            date_redraw(date_offset, date_str);
        }
        fflush(stdout);

        pthread_mutex_unlock(&mux_draw);

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
    struct timespec tspec;

    for (;;) {
        clock_gettime(CLOCK_REALTIME, &tspec);
        tspec.tv_sec++;
        err = pthread_mutex_timedlock(&mux_draw, &tspec);
        if (err) {
            // TODO handle error
            goto next_tick;
        }
        info_fetch(&tui->info);
        info_redraw(&tui->info, info_offset);
        fflush(stdout);

        pthread_mutex_unlock(&mux_draw);

    next_tick:
        /* update every INFO_REFRESH_INTERVAL seconds */
        gettimeofday(&tval, 0);
        usleep(1000000 * INFO_REFRESH_INTERVAL - tval.tv_usec);
    }

    return ((void *)0);
}
