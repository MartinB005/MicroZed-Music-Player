#pragma once
#include <functional>

class Executor {

    public:

        void schedule(std::function<void()> func, double interval, bool highPriority = false);
        void runAfter(std::function<void()> func, double delay, bool onlyOnce);
        void stopAll();
};