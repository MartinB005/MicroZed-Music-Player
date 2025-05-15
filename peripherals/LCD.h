#pragma once
#include <ui/View.h>

#define WIDTH 480
#define HEIGHT 320

class LCD {
    public:
        void init();
        void setView(View* view);
        void whiteScreen();
        void renderText(char* text, int x, int y, int color, int background);
        void renderRect(int pos_x, int pos_y, int width, int height, int color);
        void setView(View view, int x, int y);
        void update();

        View* getCurrentView();
        int getLineHeight();

    private:
        View* currentView = NULL;

        int getCharPixel(char c, int x, int y, int color, int background);
        int getWordPixel(char* str, int x, int y, int color, int background);
};

