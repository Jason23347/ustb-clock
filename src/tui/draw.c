#include "conf.h"

#include "calc.h"
#include "color.h"
#include "draw.h"
#include <string.h>

inline void
draw_space(int length) {
    for (int i = 0; i < length; i++) {
        printf(" ");
    }
}

void
draw_line(offset_t offset, const char *prompt, const calc_t *value,
          int length) {
    gotopos(offset);
    printf("%s", prompt);
    draw_space(length - strlen(prompt) - strlen(value->str));
    set_color(value->color);
    printf("%s", value->str);
    reset_color();
}

#ifndef CLOCK_DOT_WIDTH
#define CLOCK_DOT_WIDTH 2
#endif /* CLOCK_DOT_WIDTH */

#if (CLOCK_DOT_WIDTH == 1)
#define EMP " "
#elif (CLOCK_DOT_WIDTH == 2)
#define EMP "  "
#else
#define EMP "   "
#endif

#define DOT color(CLOCK_COLOR) EMP color(BG_NORMAL)

#if (CLOCK_CONDENSE == 0)
#define POINT (digit & 0x4000) ? (DOT " ") : (EMP " ")
#else
#define POINT (digit & 0x4000) ? DOT : EMP
#endif

inline void
__draw_digit(offset_t offset, int digit) {
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

/**
 * 绘制一个 5x3 的数字
 *
 * 下面以 DIGIT7 举例说明 magic number.
 * DIGIT7 = 0x7249，展开为2进制：
 *     0111 0010 0100 1001
 * 整理一下：
 *     0 111 001 001 001 001
 * 忽略最高位，继续展开为一个5x3的二维数组：
 * {
 *  {1, 1, 1},
 *  {0, 0, 1},
 *  {0, 0, 1},
 *  {0, 0, 1},
 *  {0, 0, 1},
 * }
 * 其中 1 代表实心点，0 代表空心，所有的 "1" 组成了数字 7 的形状。
 */
void
draw_digit(offset_t offset, int digit) {
    int map[10] = {
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
    };

    if (digit < 10 && digit >= 0) {
        __draw_digit(offset, map[digit]);
    } else { // :
        __draw_digit(offset, 0x0410);
    }
}

void
date_redraw(offset_t offset, const char *date_str) {
    gotopos(offset);
    printf("%s", date_str);
}
