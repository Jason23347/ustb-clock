#include "conf.h"
#if CLOCK_TYPE == CLOCK_TYPE_RANDMAP

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "draw.h"
#include "randmap.h"

offset_t digits_offset;

int clock_digit[4] = {-1, -1, -1, -1};

inline void
__digits_add_dots(digits_t *digits, int num) {
    int i, tmp;
    offset_t pos;

#define dot (digits->cur_dot)
    for (i = 0; i < num; i++) {
        /* 一维数组映射到二位坐标 */
        tmp = dot - digits->dots;
        pos = digits_offset;
        transpos(pos, (tmp % CLOCK_DOT_ROW) * CLOCK_DIGIT_WIDTH,
                 tmp / CLOCK_DOT_ROW);

        draw_dot(pos);

        dot = dot->next;
    }
#undef dot
    digits->act_num += num;
}

int
digits_init(digits_t *digits) {
    const struct tm *tm_tmp;
    const struct digitdot *dot;

    /* 洗白白 */
    memset(digits, 0, sizeof(digits_t));

    /* 打乱顺序 */
    digits->head = digitdot_shuffle(digits->dots, CLOCK_DIGIT_NUM);

    /* 计算当前act_num */
    gettimeofday(&digits->tval, 0);
    tm_tmp = localtime(&digits->tval.tv_sec);
    digits->act_num = tm_tmp->tm_hour * 10 + tm_tmp->tm_min / 6;

    dot = digits->head;
    for (size_t i = 0; i < digits->act_num; i++) {
        dot = dot->next;
    }
    digits->cur_dot = dot;

    return 0;
}

/** @return 0 if not updated, 1 if updated. */
int
__digit_update(size_t pos, int num) {
    if (clock_digit[pos] == num)
        return 0;
    clock_digit[pos] = num;
    return 1;
}

/**
 * 从右到左绘制数字，不包括分隔符（冒号）
 *
 * @return 更新的数字位数
 */
int
digits_update(digits_t *digits, const struct timeval *new_time) {
    struct tm *tmp = localtime(&new_time->tv_sec);

    /* 偶尔会发生的情况 */
    if (unlikely(new_time->tv_sec == digits->tval.tv_sec)) {
        return 0;
    }

    /* 检查是否需要更新 TUI */
    if (tmp->tm_min % 6 == 0) {
        if (draw_timedlock()) {
            return -1;
        }
        /* 加个点 */
        __digits_add_dots(digits, 1);
        draw_end();
        draw_unlock();
    }

    // 从右往左
    if (!__digit_update(0, tmp->tm_min % 10)) {
        return 0;
    } else if (!__digit_update(1, tmp->tm_min / 10)) {
        return 1;
    } else {
        /* 12小时制 */
        if (tmp->tm_hour > 12) {
            tmp->tm_hour %= 12;
        } else if (!__digit_update(2, tmp->tm_hour % 10)) {
            return 2;
        } else if (!__digit_update(3, tmp->tm_hour / 10)) {
            return 3;
        } else {
            return 4;
        }
    }

    /* Never reach */
    return 4;
}

void
digits_setpos(size_t x, size_t y) {
    setpos(digits_offset, x, y);
}

/* 从右到左绘制数字和分隔符（冒号） */
void
digits_redraw(digits_t *digits, const struct timeval *new_time) {
    int tmp;
    offset_t pos;
    const struct tm *tm_tmp;
    const struct digitdot *dot;

    digits->tval.tv_sec = new_time->tv_sec;
    digits->tval.tv_usec = new_time->tv_usec;

    /* 计算当前act_num */
    gettimeofday(&digits->tval, 0);
    tm_tmp = localtime(&digits->tval.tv_sec);
    digits->act_num = tm_tmp->tm_hour * 10 + tm_tmp->tm_min / 6;

    dot = digits->head;
    for (size_t i = 0; i < digits->act_num; i++) {
        dot = dot->next;
    }
    digits->cur_dot = dot;

    if (draw_timedlock()) {
        return;
    }

    dot = digits->head;
    for (size_t i = 0; i < digits->act_num; i++) {
        /* 一维数组映射到二位坐标 */
        tmp = dot - digits->dots;
        pos = digits_offset;
        transpos(pos, (tmp % CLOCK_DOT_ROW) * CLOCK_DIGIT_WIDTH,
                 tmp / CLOCK_DOT_ROW);

        draw_dot(pos);
        dot = dot->next;
    }

    draw_end();
    draw_unlock();
}

#endif /* CLOCK_TYPE */
