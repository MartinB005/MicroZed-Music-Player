#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <functional>
#include "Executor.h"

volatile bool running;

typedef struct {
    std::function<void()> action;
    double interval;
} IntervalArgs;

void* timer_thread(void* arg) {
    struct timespec next;
    IntervalArgs* args = (IntervalArgs*) arg;
    int ns = args->interval * 1000;

    clock_gettime(CLOCK_MONOTONIC, &next);

    while (running) {

        args->action();

        next.tv_nsec += ns;
        if (next.tv_nsec >= 1000000000) {
            next.tv_sec += next.tv_nsec / 1000000000;
            next.tv_nsec %= 1000000000;
        }

        struct timespec now;
        do {
            clock_gettime(CLOCK_MONOTONIC, &now);
        } while ((now.tv_sec < next.tv_sec) ||
                 (now.tv_sec == next.tv_sec && now.tv_nsec < next.tv_nsec));
    }

    return NULL;
}


void Executor::schedule(std::function<void()> func, double interval) {
    pthread_t thread;
    running = true;

    IntervalArgs* args = new IntervalArgs{func, interval};
    
    if (pthread_create(&thread, NULL, timer_thread, args) != 0) {
        perror("Failed to create thread");
        return;
    }

    pthread_detach(thread);
}

void Executor::stopAll() {
    running = false;
}