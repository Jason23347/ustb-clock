#ifndef CONF_H
#define CONF_H

#include "config.h"
#include "stdio.h"

#include "tui/color.h"

#ifndef NDEBUG
#define debug(...)                                                             \
    {                                                                          \
        savecursor();                                                          \
        gotoxy(0, 0);                                                          \
        printf(__VA_ARGS__);                                                   \
        loadcursor();                                                          \
    }
#else
#define debug(...)
#endif

#define LOGIN_HOST "202.204.48.82"
#define PORT       80

#define CLOCK_COLOR BG_DARKGREEN
/* Should between 1 - 6 */
#define CLOCK_DOT_WIDTH   2
#define CLOCK_SPACE_WIDTH 3
#define CLOCK_CONDENSE    0
#define CLOCK_INFO_WIDTH  28
#define CLOCK_DATE_LEN    28
#define CLOCK_DATE_FORMAT "%a %b %d  %p"

#define INFO_UPDATE_INTERVAL 3

#endif /* CONF_H */