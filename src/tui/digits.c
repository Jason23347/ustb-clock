#include "conf.h"

#include "digits.h"
#include "draw.h"

#include <string.h>
#include <time.h>

int clock_digit[4] = {-1, -1, -1, -1};

const char *
current_date(char str[CLOCK_DATE_LEN]) {
    time_t arr[1], *cur_time = &arr[0];
    time(cur_time);
    strftime(str, CLOCK_DATE_LEN, CLOCK_DATE_FORMAT, localtime(cur_time));
    return str;
}

int
clock_init(digits_t *clock) {
    clock->tval = &clock->tval_arr[0];
    return 0;
}

#define draw_digit_update(pos, num)                                            \
    {                                                                          \
        if (clock_digit[pos] == num)                                           \
            return pos;                                                        \
        draw_digit(offset, num);                                               \
    }

/**
 * 从右到左绘制数字，不包括分隔符（冒号）
 *
 * @return 更新的数字位数
 */
int
clock_update(digits_t *clock, struct timeval *new_time, offset_t offset) {
    struct tm *tmp = localtime(&new_time->tv_sec);

    // 从右往左
    transpos(offset, (CLOCK_MIN_WIDTH - CLOCK_DIGIT_WIDTH), 0);

    draw_digit_update(0, tmp->tm_min % 10);
    transpos(offset, -(CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH), 0);
    draw_digit_update(1, tmp->tm_min / 10);

    // 跳过分隔符（冒号）
    transpos(offset, -2 * (CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH - 2), 0);

    /* 12小时制 */
    if (tmp->tm_hour > 12) {
        tmp->tm_hour %= 12;
    }

    draw_digit_update(2, tmp->tm_hour % 10);
    transpos(offset, -(CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH), 0);
    draw_digit_update(3, tmp->tm_hour / 10);

    clock->tval = new_time;

    return 4;
}

/* 从右到左绘制数字和分隔符（冒号） */
void
clock_redraw(digits_t *clock, struct timeval *new_time, offset_t offset) {
    memset(&clock_digit, -1, sizeof(clock_digit));
    clock_update(clock, new_time, offset);

    transpos(offset, 2 * (CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH) - 2, 0);
    draw_digit(offset, -1);
}
