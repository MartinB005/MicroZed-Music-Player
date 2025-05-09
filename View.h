#pragma once
#include "icons.h"
#include <functional>

class View {

    public:
        View(int width, int height);
        void setWriteDest(unsigned int* addr, std::function<void()> func);    
        void update();
        void freeze();
        unsigned int* getPixels();
        int getWidth();
        int getHeight();
    
    private:
        unsigned int* pixels;
        std::function<void()> onUpdate;
        bool frozen = false;

    protected:
        int width, height;
        void renderText(char* text, int x, int y, int color, int background);
        void renderRect(int pos_x, int pos_y, int width, int height, int color);
        void renderIcon(icon icn, int x, int y, float scale);
        int getCharPixel(char c, int x, int y, int color, int background);
        int getWordPixel(char* str, int x, int y, int color, int background); 
        int getLineHeight();
};