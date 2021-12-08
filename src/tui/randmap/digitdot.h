#ifndef DIGITDOT_H
#define DIGITDOT_H

#include "bits/types.h"

struct digitdot {
    __uint8_t activated;
    struct digitdot *next;
};

#endif /* DIGITDOT_H */