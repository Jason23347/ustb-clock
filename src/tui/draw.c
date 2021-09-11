#include "conf.h"

#include "draw.h"
#include <string.h>

void
draw_line(struct offset offset, const char *prompt, const char *value,
          int length) {
    gotopos(offset);
    printf("%s", prompt);
    goright(length - strlen(prompt) - strlen(value));
    printf("%s", value);
}

#ifdef CLOCK_DOT_WIDTH
#if     (CLOCK_DOT_WIDTH == 1)
#define EMP         " "
#elif   (CLOCK_DOT_WIDTH == 2)
#define EMP         "  "
#elif   (CLOCK_DOT_WIDTH == 3)
#define EMP         "   "
#elif   (CLOCK_DOT_WIDTH == 4)
#define EMP         "    "
#elif   (CLOCK_DOT_WIDTH == 5)
#define EMP         "     "
#elif   (CLOCK_DOT_WIDTH == 6)
#define EMP         "      "
#endif
#define DOT CLOCK_COLOR EMP "\033[0m"
#endif /* CLOCK_DOT_WIDTH */

#if (CLOCK_CONDENSE == 0)
#define POINT (map[i][j]) ? (DOT " ") : (EMP " ")
#else
#define POINT (map[i][j]) ? DOT : EMP
#endif

void
__draw_digit(struct offset offset, int map[5][3]) {
    for (int i = 0; i < 5; i++) {
        gotopos(offset);
        for (int j = 0; j < 3; j++) {
            printf(POINT);
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
    /* 枚举大法就是好 */
    if (digit == 0) {
        int map[5][3] = {
            {1, 1, 1},
            {1, 0, 1},
            {1, 0, 1},
            {1, 0, 1},
            {1, 1, 1},
        };
        __draw_digit(offset, map);
    } else if (digit == 1) {
        int map[5][3] = {
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
            {0, 1, 0},
        };
        __draw_digit(offset, map);
    } else if (digit == 2) {
        int map[5][3] = {
            {1, 1, 1},
            {0, 0, 1},
            {1, 1, 1},
            {1, 0, 0},
            {1, 1, 1},
        };
        __draw_digit(offset, map);
    } else if (digit == 3) {
        int map[5][3] = {
            {1, 1, 1},
            {0, 0, 1},
            {1, 1, 1},
            {0, 0, 1},
            {1, 1, 1},
        };
        __draw_digit(offset, map);
    } else if (digit == 4) {
        int map[5][3] = {
            {1, 0, 1},
            {1, 0, 1},
            {1, 1, 1},
            {0, 0, 1},
            {0, 0, 1},
        };
        __draw_digit(offset, map);
    } else if (digit == 5) {
        int map[5][3] = {
            {1, 1, 1},
            {1, 0, 0},
            {1, 1, 1},
            {0, 0, 1},
            {1, 1, 1},
        };
        __draw_digit(offset, map);
    } else if (digit == 6) {
        int map[5][3] = {
            {1, 1, 1},
            {1, 0, 0},
            {1, 1, 1},
            {1, 0, 1},
            {1, 1, 1},
        };
        __draw_digit(offset, map);
    } else if (digit == 7) {
        int map[5][3] = {
            {1, 1, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
            {0, 0, 1},
        };
        __draw_digit(offset, map);
    } else if (digit == 8) {
        int map[5][3] = {
            {1, 1, 1},
            {1, 0, 1},
            {1, 1, 1},
            {1, 0, 1},
            {1, 1, 1},
        };
        __draw_digit(offset, map);
    } else if (digit == 9) {
        int map[5][3] = {
            {1, 1, 1},
            {1, 0, 1},
            {1, 1, 1},
            {0, 0, 1},
            {0, 0, 1},
        };
        __draw_digit(offset, map);
    } else { // 冒号
        int map[5][3] = {
            {0, 0, 0},
            {0, 1, 0},
            {0, 0, 0},
            {0, 1, 0},
            {0, 0, 0},
        };
        __draw_digit(offset, map);
    }
}