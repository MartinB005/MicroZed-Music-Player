#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <functional>
#include "Executor.h"

volatile bool running;
volatile int counter;

typedef struct {
    std::function<void()> action;
    double time;
    bool onlyOnce;
} ActionArgs;

void* timer_thread(void* arg) {
    struct timespec next;
    ActionArgs* args = (ActionArgs*) arg;
    int ns = args->time * 1000;

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


void Executor::schedule(std::function<void()> func, double interval, bool highPriority) {
    pthread_t thread;
    running = true;

    ActionArgs* args = new ActionArgs{func, interval, false};
    pthread_attr_t* attrPtr = NULL;
    
    if (highPriority) {
        pthread_attr_t attr;
        struct sched_param param;

        pthread_attr_init(&attr);
        pthread_attr_setschedpolicy(&attr, SCHED_FIFO); 

        param.sched_priority = 60; 
        pthread_attr_setschedparam(&attr, &param);
        pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
        attrPtr = &attr;
    }
        
    if (pthread_create(&thread, attrPtr, timer_thread, args) != 0) {
        perror("Failed to create thread");
        return;
    }

    pthread_detach(thread);
}

void* run(void* arg) {
    struct timespec next;
    ActionArgs* args = (ActionArgs*) arg;

    counter++;
    int counterVal = counter;
    
    usleep((int) args->time);

    if (!args->onlyOnce || counterVal == counter) {
        args->action();
    }

    return NULL;
}

void Executor::runAfter(std::function<void()> func, double delay, bool onlyOnce) {
    pthread_t thread;
    ActionArgs* args = new ActionArgs{func, delay, onlyOnce};
  
    if (pthread_create(&thread, NULL, run, args) != 0) {
        perror("Failed to create thread");
        return;
    }

    pthread_detach(thread);
}

void Executor::stopAll() {
    running = false;
}