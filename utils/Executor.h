#pragma once
#include <functional>

class Executor {

    public:
        
        /**
         * @brief Schedule new timed action to the executor
         * 
         * @param func callback of the action
         * @param interval interval in microseconds
         * @param highPriority whether thread should have a high priority 
         */
        void schedule(std::function<void()> func, double interval, bool highPriority = false);

        /**
         * @brief Run action after specified time on another thread
         * 
         * @param func callback of the aciton
         * @param delay when the action should be run (microseconds)
         * @param onlyOnce 
         */
        void runAfter(std::function<void()> func, double delay, bool onlyOnce);

        /**
         * @brief Stop all scheduled actions
         */
        void stopAll();
};