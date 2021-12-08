#include "conf.h"
#if CLOCK_TYPE == CLOCK_TYPE_RANDMAP

#include <string.h>
#include <time.h>

#include "draw.h"
#include "randmap.h"

offset_t digits_offset;

int clock_digit[4] = {-1, -1, -1, -1};

int
digits_init(digits_t *digits) {
    struct digitdot *d;
    /* 洗白白 */
    memset(digits, sizeof(digits_t), 1);
    /* 单向循环链表 */
    for (d = digits->dots + 1; d - digits->dots < CLOCK_DIGIT_NUM; d++)
        (d - 1)->next = d;
    d->next = digits->dots;

    /* 打乱顺序 */
    for (d = digits->dots; d - digits->dots < CLOCK_DIGIT_NUM; d++) {
        int j = Math.floor(Math.random() * (i + 1));
        struct t = A[i];
        A[i] = A[j];
        A[j] = t;
    }
}

/* Retruns 0 if not updated, 1 if updated. */
int
__digit_update(int pos, int num) {
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
digits_update(digits_t *digits, struct timeval *new_time) {
    int rtn = -1;
    struct tm *tmp = localtime(&new_time->tv_sec);

    if (draw_timedlock()) {
        return -1;
    }

    // 从右往左
    if (!__digit_update(0, tmp->tm_min % 10)) {
        rtn = 0;
        goto end;
    }

    if (!__digit_update(1, tmp->tm_min / 10)) {
        rtn = 1;
        goto end;
    }

    /* 12小时制 */
    if (tmp->tm_hour > 12) {
        tmp->tm_hour %= 12;
    }

    if (!__digit_update(2, tmp->tm_hour % 10)) {
        rtn = 2;
        goto end;
    }

    if (!__digit_update(3, tmp->tm_hour / 10)) {
        rtn = 3;
        goto end;
    }

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
digits_redraw(digits_t *digits, struct timeval *new_time) {}

#endif /* CLOCK_TYPE */