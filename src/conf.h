#ifndef CONF_H
#define CONF_H

#include "config.h"
#include "stdio.h"
#define debug(...) fprintf(stderr, __VA_ARGS__)

#define LOGIN_HOST "202.204.48.82"
#define PORT 80

#define CLOCK_COLOR "\033[46m"
#define CLOCK_DIDGET_WIDTH 2
#define CLOCK_SPACE_WIDTH 3
#define CLOCK_INFO_WIDTH 28
#define CLOCK_DATE_FORMAT "%a %b %d  %p"
#define CLOCK_DATE_LEN 28

#endif /* CONF_H */