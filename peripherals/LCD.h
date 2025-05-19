#pragma once
#include <ui/View.h>

#define WIDTH 480
#define HEIGHT 320

class LCD {
    public:
    
        /**
         * @brief map the base memory to control the display and prepare it for write
         */
        void init();

        /**
         * @brief Set the View which should be displayed
         * 
         * @param view the View object pointer
         */
        void setView(View* view);

        /**
         * @brief show white screen on the display
         */
        void whiteScreen();

        /**
         * @brief apply written data to the display output        * 
         */
        void update();

        View* getCurrentView();
        int getLineHeight();

    private:
        View* currentView = NULL;
};

