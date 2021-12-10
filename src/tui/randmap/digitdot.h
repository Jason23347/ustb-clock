#ifndef DIGITDOT_H
#define DIGITDOT_H

#include "bits/types.h"
#include <stddef.h>

struct digitdot {
    struct digitdot *next;
};

struct digitdot *digitdot_shuffle(struct digitdot *arr, size_t len);

#endif /* DIGITDOT_H */