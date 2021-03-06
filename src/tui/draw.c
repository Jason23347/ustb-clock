#include "conf.h"

#include "calc.h"
#include "draw.h"

#include <pthread.h>
#include <string.h>
#include <sys/time.h>

pthread_mutex_t mtx_draw;

inline void
__draw_space(ssize_t length) {
    for (size_t i = 0; i < length; i++) {
        printf(" ");
    }
}

void
draw_line(offset_t offset, const char *prompt, const calc_t *value,
          size_t length) {
    gotopos(offset);
    printf("%s", prompt);
    __draw_space(length - strlen(prompt) - strlen(value->str));
    set_color(value->color);
    printf("%s", value->str);
    reset_color();
}

/** Draw centered text */
#ifdef COLORFUL_OUTPUT
void
draw_center(offset_t offset, const char *str, int color, size_t length) {
#else
void
draw_center(offset_t offset, const char *str, size_t length) {
#endif /* COLORFUL_OUTPUT */
    int space_len = (length - strlen(str)) / 2;
    gotopos(offset);
    __draw_space(space_len);
    set_color(color);
    printf("%s", str);
    reset_color();
    __draw_space(length - space_len);
}

void
draw_empty_line(offset_t offset, size_t length) {
    gotopos(offset);
    __draw_space(length);
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

#if CLOCK_TYPE == CLOCK_TYPE_DIGITS
#if (CLOCK_CONDENSE == 0)
#define POINT (digit & 0x4000) ? (DOT " ") : (EMP " ")
#else
#define POINT (digit & 0x4000) ? DOT : EMP
#endif /* CLOCK_CONDENSE */

#if (CLOCK_CONDENSE == 0)
#define __draw_digit_next_line(offset) transpos(offset, 0, 2)
#else
#define __draw_digit_next_line(offset) next_line(offset)
#endif

inline void
__draw_digit(offset_t offset, int digit) {
    for (int i = 0; i < 5; i++) {
        gotopos(offset);
        for (int j = 0; j < 3; j++) {
            printf(POINT);
            digit <<= 1;
        }
        __draw_digit_next_line(offset);
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
draw_digit(offset_t offset, size_t digit) {
    int map[10] = {
        0x7B6F, // 0
        0x1649, // 1
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
#endif /* CLOCK_TYPE_DIGITS */

void
draw_dot(offset_t offset) {
    gotopos(offset);
    printf(DOT);
}

int
draw_lock_init() {
    return pthread_mutex_init(&mtx_draw, 0);
}

int
draw_lock() {
    return pthread_mutex_lock(&mtx_draw);
}

int
draw_timedlock() {
    struct timespec tspec;

    clock_gettime(CLOCK_REALTIME, &tspec);
    tspec.tv_sec++;
    return pthread_mutex_timedlock(&mtx_draw, &tspec);
}

int
draw_unlock() {
    return pthread_mutex_unlock(&mtx_draw);
}

void
draw_start() {}

void
draw_end() {
    hidecursor();
    if (flush())
        flush();
}
