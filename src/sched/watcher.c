#include "watcher.h"

#include "tui/draw.h"
#include <string.h>

const watcher_t *
watcher_init(watcher_t *watcher) {
    memset(watcher, 0, sizeof(watcher_t));

    pthread_mutex_init(&watcher->mtx, 0);
    pthread_cond_init(&watcher->cond, 0);

    return watcher;
}

void
watcher_register(watcher_t *watcher, task_t task) {
    memcpy(&watcher->task_list[watcher->len], &task, sizeof(task_t));
    watcher->len++;
}

void *
watcher_schedule(void *arg) {
    watcher_t *watcher = arg;
    task_sort(watcher->task_list, watcher->len);

    for (int least = watcher->task_list->num;;) {
        pthread_mutex_lock(&watcher->mtx);
        while (watcher->num == 0) {
            pthread_cond_wait(&watcher->cond, &watcher->mtx);
        }
        // Schedule tasks
        for (const task_t *task = watcher->task_list;
             task - watcher->task_list < watcher->len; task++) {
            if (watcher->num < least) {
                continue;
            }
            if (task->num <= watcher->num) {
                task_run(task, watcher->num);
            }
        }
        watcher->num = 0;
        pthread_mutex_unlock(&watcher->mtx);
    }
}

int
watcher_awake(watcher_t *watcher) {
    return pthread_cond_signal(&watcher->cond);
}