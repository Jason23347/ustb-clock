#ifndef BUFF_H
#define BUFF_H

#include "src/conf.h"

#include <stddef.h>

typedef struct {
    char *str;
    size_t len;
    size_t max;
} buff_t;

int buff_init(buff_t *buff, size_t length);
void buff_free(buff_t *buff);
int buff_resize(buff_t *buff, size_t length);

#endif