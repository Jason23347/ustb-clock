#ifndef TASK_H
#define TASK_H

#define MINUTELY          1
#define EVERY_TEN_MINUTES 2
#define HOURLY            3
#define DAILY             4

#include <stddef.h>

typedef struct {
    int num;
    void *(*func)(int);
} task_t;

const task_t *task_init(task_t *task, int num, void *(*)(int));
void *task_run(const task_t *task, int num);
void task_sort(task_t *task_list, size_t num);

#endif /* TASK_H */