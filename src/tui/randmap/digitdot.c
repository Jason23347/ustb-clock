#include "conf.h"

#include <stdlib.h>
#include <time.h>

#include "digitdot.h"

struct digitdot *
digitdot_shuffle(struct digitdot *arr, size_t len) {
    struct digitdot *a;
    struct digitdot **p, **q, **pos, *tmp;

    pos = malloc(len * sizeof(struct digitdot *));

    for (a = arr, p = pos; a - arr < len; a++, p++) {
        *p = a;
    }

    /* Shuffle pos */
    srand(time(0));
    for (p = pos; p - pos < len; p++) {
        q = pos + rand() % len;

        tmp = *q;
        *q = *p;
        *p = tmp;
    }

    for (p = pos; p - pos < len - 1; p++) {
        (*p)->next = *(p + 1);
    }
    (*p)->next = *pos;

    free(pos);

    return arr;
}
