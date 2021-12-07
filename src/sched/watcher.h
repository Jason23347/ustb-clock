#ifndef WATCHER_H
#define WATCHER_H

#include "task.h"
#include <pthread.h>

#define MAX_TASK_NUM  8
#define COUNT_OF(arr) (sizeof(arr) / sizeof(0 [arr]))

typedef struct {
    int num;
    int len;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    task_t task_list[MAX_TASK_NUM];
} watcher_t;

const watcher_t *watcher_init(watcher_t *watcher);
void watcher_register(watcher_t *watcher, task_t task);
void *watcher_schedule(void *arg);
int watcher_awake(watcher_t *watcher);

#endif /* WATCHER_H */