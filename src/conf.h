#ifndef CONF_H
#define CONF_H

#include "color.h"
#include "config.h"

#include <sys/types.h>

#if __WORDSIZE == 64
#define size_spec "%lu"
#define ssize_spec "%ld"
#define u_int64_spec "%lu"
#else
#define size_spec "%u"
#define ssize_spec "%d"
#define u_int64_spec "%llu"
#endif /* __WORDSIZE */

#ifndef NDEBUG
#define debug(...)                                                             \
    {                                                                          \
        savecursor();                                                          \
        gotoxy(0, 0);                                                          \
        printf(__VA_ARGS__);                                                   \
        loadcursor();                                                          \
    }

#if __GNUC__ > 3
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#endif /* __GNUC__ */

#else

#define debug(...)
#define likely(x)   (x)
#define unlikely(x) (x)
#endif /* NDEBUG */

#define LOGIN_HOST "202.204.48.82"
#define PORT       80

#define SOCKET_TIMEOUT 3

#define CLOCK_COLOR       BG_DARKGREEN
#define CLOCK_DOT_WIDTH   2
#define CLOCK_SPACE_WIDTH 3
#define CLOCK_CONDENSE    0
#define CLOCK_INFO_WIDTH  28
#define CLOCK_DATE_LEN    28
#define CLOCK_DATE_FORMAT "%a %b %d  %p"

#define INFO_REFRESH_INTERVAL 1

#define CLOCK_TYPE_DIGITS  0 /* default */
#define CLOCK_TYPE_RANDMAP 1
#ifndef CLOCK_TYPE
#define CLOCK_TYPE CLOCK_TYPE_DIGITS
#endif

#if (CLOCK_TYPE == CLOCK_TYPE_RANDMAP) && CLOCK_CONDENSE
#undef CLOCK_CONDENSE
#define CLOCK_CONDENSE 0
#endif

#endif /* CONF_H */
