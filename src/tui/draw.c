#include "conf.h"

#include "calc.h"
#include "color.h"
#include "draw.h"
#include <string.h>

void
draw_line(struct offset offset, const char *prompt, const calc_t *value,
          int length) {
    gotopos(offset);
    printf("%s", prompt);
    goright(length - strlen(prompt) - strlen(value->str));
    printf("\033[%dm%s", value->color, value->str);
    reset_color();
}

#ifdef CLOCK_DOT_WIDTH
#if (CLOCK_DOT_WIDTH == 1)
#define EMP " "
#elif (CLOCK_DOT_WIDTH == 2)
#define EMP "  "
#elif (CLOCK_DOT_WIDTH == 3)
#define EMP "   "
#elif (CLOCK_DOT_WIDTH == 4)
#define EMP "    "
#elif (CLOCK_DOT_WIDTH == 5)
#define EMP "     "
#elif (CLOCK_DOT_WIDTH == 6)
#define EMP "      "
#endif
#define DOT CLOCK_COLOR EMP "\033[0m"
#endif /* CLOCK_DOT_WIDTH */

#if (CLOCK_CONDENSE == 0)
#define POINT (digit & 0x4000) ? (DOT " ") : (EMP " ")
#else
#define POINT (digit & 0x4000) ? DOT : EMP
#endif

void
__draw_digit(struct offset offset, int digit) {
    for (int i = 0; i < 5; i++) {
        gotopos(offset);
        for (int j = 0; j < 3; j++) {
            printf(POINT);
            digit <<= 1;
        }
#if (CLOCK_CONDENSE == 0)
        transpos(offset, 0, 2);
#else
        next_line(offset);
#endif
    }
}

void
draw_digit(struct offset offset, int digit) {
    int map[11] = {
        0x7B6F, // 0
        0x2492, // 1
        0x73E7, // 2
        0x73CF, // 3
        0x5BC9, // 4
        0x79CF, // 5
        0x79EF, // 6
        0x7249, // 7
        0x7BEF, // 8
        0x7BC9, // 9
        0x0410, // :
    };

    if (digit < 10 && digit >= 0) {
        __draw_digit(offset, map[digit]);
    } else { // 冒号
        __draw_digit(offset, map[10]);
    }
}

void
date_redraw(struct offset offset, const char *date_str) {
    gotopos(offset);
    printf("%s", date_str);
}