#include "conf.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "tui/tui.h"

const char *
clock_type(int type) {
    switch (type) {
    case CLOCK_TYPE_RANDMAP:
        return "randmap";
    case CLOCK_TYPE_DIGITS:
    default:
        return "digits";
    }
}

void
print_info() {
    printf(PACKAGE_STRING
           "\n"

#ifdef COLORFUL_OUTPUT
           "+color "
#else
           "-color "
#endif /* COLORFUL_OUTPUT */

#ifndef NDEBUG
           "+debug "
#else
           "-debug "
#endif /* NDEBUG */

#ifdef ALWAYS_DETECT_WINSIZE
           "+winsize "
#else
           "-winsize "
#endif /* ALWAYS_DETECT_WINSIZE */

           "\n\n"
           "Copy 2021 Jason <jason23347@gmail.com>\n\n"
           "Login host:\t%s\n"
           "Login port:\t%d\n"
           "Clock color:\t" color(CLOCK_COLOR) "  " color(
               BG_NORMAL) " %s\n"
                          "Clock type:\t%s\n"
                          "Dot width:\t%d\n"
                          "Space width:\t%d\n"
                          "Condense mode:\t%s\n"
                          "Info refresh interval: %d second(s)\n\n",
           LOGIN_HOST, PORT, colorname(CLOCK_COLOR), clock_type(CLOCK_TYPE),
           CLOCK_DOT_WIDTH, CLOCK_SPACE_WIDTH,
#ifdef COLORFUL_OUTPUT
           CLOCK_CONDENSE ? color(GREEN) "TRUE" color(NORMAL)
                          : color(RED) "FALSE" color(NORMAL),
#else
           CLOCK_CONDENSE ? "TRUE" : "FALSE",
#endif /* COLORFUL_OUTPUT */
           INFO_REFRESH_INTERVAL);
}

int
main(int argc, char *const *argv) {
    int c;

    while ((c = getopt(argc, argv, "h")) != -1) {
        switch (c) {
        case 'h':
            print_info();
            return 0;
        default:
            fprintf(stderr, "Unknown option: %c\n", c);
            return -1;
        }
    }

    /* disable stderr */
    fclose(stderr);

    tui_init();

    return 0;
}
