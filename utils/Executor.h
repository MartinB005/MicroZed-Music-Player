#pragma once
#include <functional>

class Executor {

    public:

        void schedule(std::function<void()> func, double interval);
        void stopAll();
};