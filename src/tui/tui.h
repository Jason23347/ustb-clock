#ifndef TUI_H
#define TUI_H

#include <stdio.h>

#include "digits.h"
#include "info.h"
#include "sys/ioctl.h"

typedef struct {
    digits_t clock;
    info_t info;
    struct winsize win;
} tui_t;

int tui_init();
void *tui_redraw(int num);

void *digits_schedule(void *arg);
void *info_schedule(void *arg);

#endif /* TUI_H */