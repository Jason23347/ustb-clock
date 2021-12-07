#include "conf.h"

#include "digits.h"

#include <string.h>
#include <time.h>

offset_t digits_offset;

int clock_digit[4] = {-1, -1, -1, -1};

int
digits_init(digits_t *digits) {
    digits->tval = &digits->tval_arr[0];
    return 0;
}

/* Retruns 0 if not updated, 1 if updated. */
int
__digit_update(offset_t offset, int pos, int num) {
    if (clock_digit[pos] == num)
        return 0;
    clock_digit[pos] = num;
    draw_digit(offset, num);

    return 1;
}

/**
 * 从右到左绘制数字，不包括分隔符（冒号）
 *
 * @return 更新的数字位数
 */
int
digits_update(digits_t *digits, struct timeval *new_time) {
    int rtn = -1;
    offset_t offset = digits_offset;
    struct tm *tmp = localtime(&new_time->tv_sec);

    if (draw_timedlock()) {
        return -1;
    }

    // 从右往左
    transpos(offset, (CLOCK_MIN_WIDTH - CLOCK_DIGIT_WIDTH), 0);
    if (!__digit_update(offset, 0, tmp->tm_min % 10)) {
        rtn = 0;
        goto end;
    }

    transpos(offset, -(CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH), 0);
    if (!__digit_update(offset, 1, tmp->tm_min / 10)) {
        rtn = 1;
        goto end;
    }

    // 跳过分隔符（冒号）
    transpos(offset, -2 * (CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH - CLOCK_DOTS_OFFSET), 0);

    /* 12小时制 */
    if (tmp->tm_hour > 12) {
        tmp->tm_hour %= 12;
    }

    if (!__digit_update(offset, 2, tmp->tm_hour % 10)) {
        rtn = 2;
        goto end;
    }

    transpos(offset, -(CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH), 0);
    if (!__digit_update(offset, 3, tmp->tm_hour / 10)) {
        rtn = 3;
        goto end;
    }

    digits->tval = new_time;
    rtn = 4;

end:
    draw_end();
    draw_unlock();

    return rtn;
}

void
digits_setpos(int x, int y) {
    setpos(digits_offset, x, y);
}

/* 从右到左绘制数字和分隔符（冒号） */
void
digits_redraw(digits_t *digits, struct timeval *new_time) {
    offset_t offset = digits_offset;

    memset(&clock_digit, -1, sizeof(clock_digit));
    digits_update(digits, new_time);

    transpos(offset, 2 * (CLOCK_DIGIT_WIDTH + CLOCK_SPACE_WIDTH) - CLOCK_DOTS_OFFSET, 0);
    draw_digit(offset, -1);

    draw_end();
}
