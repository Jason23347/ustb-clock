#include "buff.h"
#include "stdlib.h"
#include "stdio.h"

#define buff_error(str) fprintf(stderr, "%s: " str, __FUNCTION__);

int
buff_init(buff_t *buff, size_t length) {
    buff->str = malloc(length);
    if (!buff->str) {
        buff_error("out of memory.\n");
        return -1;
    }

    buff->len = 0;
    buff->max = length;

    return 0;
}

void
buff_free(buff_t *buff) {
    buff->len = 0;
    buff->max = 0;
    buff->str = malloc(0);
}

int
buff_resize(buff_t *buff, size_t length) {
    void *new = realloc(buff->str, length);
    if (!new) {
        buff_error("realloc failed.\n");
        return -1;
    }

    buff->str = new;

    return 0;
}