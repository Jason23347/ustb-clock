#include "task.h"

#include <stdlib.h>

const task_t *
task_init(task_t *task, int num, void *(*func)(int)) {
    task->num = num;
    task->func = func;
    return task;
}

void *
task_run(const task_t *task, int num) {
    return task->func(num);
}

int
__task_cmp(const void *a, const void *b) {
    const task_t *t1 = a, *t2 = b;
    return (t1->num - t2->num);
}

void
task_sort(task_t *task_list, size_t num) {
    qsort(task_list, num, sizeof(task_t), __task_cmp);
}
