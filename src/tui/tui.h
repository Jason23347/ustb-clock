#ifndef TUI_H
#define TUI_H

#include <stdio.h>
#include <sys/ioctl.h>

#if CLOCK_TYPE == CLOCK_TYPE_DIGITS
#include "digits.h"
#elif CLOCK_TYPE == CLOCK_TYPE_RANDMAP
#include "randmap.h"
#endif /* CLOCK_TYPE */

#include "date.h"
#include "info.h"

typedef struct {
    digits_t clock; /* Defined in diffrent clock headers */
    info_t info;
    struct winsize win;
} tui_t;

int tui_detect_size(const tui_t *tui);
int tui_getwinsize(tui_t *tui);
int tui_init();
void *tui_redraw(int num);

// TODO sched/sched
void *digits_schedule(void *arg);
void *info_schedule(void *arg);

#endif /* TUI_H */