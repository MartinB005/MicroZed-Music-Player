#pragma once
#include "icons.h"
#include "colors.h"
#include <functional>

#define WIDTH_RATIO(val, scale) WIDTH * val / 100 - ICON_SIZE * scale / 2
#define CENTER -1

class View {

    public:
        View(int width, int height);
        void setWriteDest(unsigned int* addr, std::function<void()> func);    
        void updateScreen();
        void freeze();
        void blackScreen();
        unsigned int* getPixels();
        int getWidth();
        int getHeight();

        virtual void update() = 0;
    
    private:
        unsigned int* pixels = NULL;
        std::function<void()> onUpdate;
        bool frozen = false;

    protected:
        int width, height;

        void renderText(const char* text, int x, int y, int color, int background);
        void renderText(const char* text, int pos_x, int pos_y, int color, int background, int scale);
        void renderRect(int pos_x, int pos_y, int width, int height, int color);
        void renderIcon(icon icn, int x, int y, float scale, int color, int background = BLACK);
        int getCharPixel(char c, int x, int y, int color, int background);
        int getWordPixel(char* str, int x, int y, int color, int background); 
        int getLineHeight();
};