#ifndef TUI_H
#define TUI_H

#include <stdio.h>

#include "clock.h"
#include "info.h"
#include "sys/ioctl.h"

typedef struct {
    clock_t clock;
    info_t info;
    struct winsize winsize;
} tui_t;

int tui_init(tui_t *tui);
void tui_redraw(tui_t *tui);

#endif /* TUI_H */