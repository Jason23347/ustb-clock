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

/**
 * 绘制一个 5x3 的数字
 *
 * 下面以DIGIT7 举例说明 magic number.
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
draw_digit(struct offset offset, int digit) {
#define DIGIT0          0x7B6F // 0
#define DIGIT1          0x2492 // 1
#define DIGIT2          0x73E7 // 2
#define DIGIT3          0x73CF // 3
#define DIGIT4          0x5BC9 // 4
#define DIGIT5          0x79CF // 5
#define DIGIT6          0x79EF // 6
#define DIGIT7          0x7249 // 7
#define DIGIT8          0x7BEF // 8
#define DIGIT9          0x7BC9 // 9
#define DIGIT_SEP       0x0410 // :
#define digit(num)      DIGIT##num

    /* 枚举大法好 */
    if (digit == 0) {
        __draw_digit(offset, digit(0));
    } else if (digit == 1) {
        __draw_digit(offset, digit(1));
    } else if (digit == 2) {
        __draw_digit(offset, digit(2));
    } else if (digit == 3) {
        __draw_digit(offset, digit(3));
    } else if (digit == 4) {
        __draw_digit(offset, digit(4));
    } else if (digit == 5) {
        __draw_digit(offset, digit(5));
    } else if (digit == 6) {
        __draw_digit(offset, digit(6));
    } else if (digit == 7) {
        __draw_digit(offset, digit(7));
    } else if (digit == 8) {
        __draw_digit(offset, digit(8));
    } else if (digit == 9) {
        __draw_digit(offset, digit(9));
    } else { // 冒号
        __draw_digit(offset, DIGIT_SEP);
    }
}

void
date_redraw(struct offset offset, const char *date_str) {
    gotopos(offset);
    printf("%s", date_str);
}