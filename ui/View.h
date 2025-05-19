#pragma once
#include "icons.h"
#include "colors.h"
#include <functional>

#define WIDTH_RATIO(val, scale) WIDTH * val / 100 - ICON_SIZE * scale / 2
#define CENTER -1

class View {

    public:

        /**
         * @brief Construct a new View object
         * 
         * @param width of the view
         * @param height of the view
         */
        View(int width, int height);

        /**
         * @brief Set the destination where should the object write the output
         * 
         * @param addr pointer to the pixel buffer
         * @param func callback after view is updated
         */
        void setWriteDest(unsigned int* addr, std::function<void()> func);    

        /**
         * @brief Update the view
         */
        void updateScreen();
        
        /**
         * @brief Show the black screen
         * 
         */
        void blackScreen();

        /**
         * @brief Get the Pixels destination pointer
         */
        unsigned int* getPixels();
        int getWidth();
        int getHeight();

        /**
         * @brief Update the child object
         */
        virtual void update() = 0;
    
    private:
        unsigned int* pixels = NULL;
        std::function<void()> onUpdate;
        bool frozen = false;

    protected:
        int width, height;

        /**
         * @brief Render text on the view
         * 
         * @param text 
         * @param x position x
         * @param y position y
         * @param color fill color of the text
         * @param background color
         */
        void renderText(const char* text, int x, int y, int color, int background);

        /**
         * @brief Render text on the view
         * 
         * @param text 
         * @param x position x
         * @param y position y
         * @param color fill color of the text
         * @param background color
         * @param scale text scaled size
         */
        void renderText(const char* text, int pos_x, int pos_y, int color, int background, int scale);

        /**
         * @brief Render rectangle on the view
         * 
         * @param pos_x position x
         * @param pos_y position y
         * @param width of the rectangle
         * @param height of the rectangle
         * @param color fill of the rectangle
         */
        void renderRect(int pos_x, int pos_y, int width, int height, int color);

        /**
         * @brief Render icon on the view
         * 
         * @param icn icon
         * @param x position x
         * @param y position y
         * @param scale scaled size of the icon
         * @param color fill color of the icon
         * @param background color
         */
        void renderIcon(icon icn, int x, int y, float scale, int color, int background = BLACK);

        /**
         * @brief Get the character pixel of the text
         * 
         * @param c character
         * @param x position x
         * @param y position y
         * @param color fill color
         * @param background color
         * @return int pixel color
         */
        int getCharPixel(char c, int x, int y, int color, int background);

        /**
         * @brief Get the word pixel
         * 
         * @param str text
         * @param x position x
         * @param y position y
         * @param color fill color
         * @param background color
         * @return int pixel color
         */
        int getWordPixel(char* str, int x, int y, int color, int background); 
        int getLineHeight();
};