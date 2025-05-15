#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "colors.h"
#include "font_types.h"
#include "View.h"

#define CHAR_WIDTH 10

View::View(int width, int height) {
    this->width = width;
    this->height = height;
}


void View::setWriteDest(unsigned int* addr, std::function<void()> func) {
    if (pixels != NULL) return;
    pixels = addr;
    this->onUpdate = func;

    printf("write dest %p\n", pixels);
}

unsigned int* View::getPixels() {
    return pixels;
}

void View::blackScreen() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            pixels[y * width  + x] = BLACK;
        }
    }
}

void View::updateScreen() {
    if (pixels == nullptr) {
        pixels = (unsigned int*) malloc(width * height * sizeof(unsigned int));
    }

    if (!frozen) onUpdate();
}

void View::renderText(const char* text, int pos_x, int pos_y, int color, int background) {

    if (pos_x == CENTER) pos_x = width / 2 - strlen(text) * CHAR_WIDTH / 2;

    for (int y = pos_y; y < pos_y + font_winFreeSystem14x16.height; ++y) {
        for (int x = pos_x; x < pos_x + strlen(text) * CHAR_WIDTH && x < width; ++x) {
            pixels[y * width + x] = getWordPixel((char*) text, x - pos_x, y - pos_y, color, background);
        }
    }
}

void View::renderText(const char* text, int pos_x, int pos_y, int color, int background, int scale) {
    int font_height = font_winFreeSystem14x16.height;
    int text_length = strlen(text);

    int total_width = CHAR_WIDTH * text_length;

    printf("text %s %d\n", text, strlen(text));

    if (pos_x == CENTER) pos_x = width / 2 - strlen(text) * CHAR_WIDTH * scale / 2;

    for (int y = 0; y < font_height; ++y) {
        for (int x = 0; x < total_width && x < this->width; ++x) {
            int pixelColor = getWordPixel((char*) text, x, y, color, background);

            for (int dy = 0; dy < scale; ++dy) {
                for (int dx = 0; dx < scale; ++dx) {
                    int sx = pos_x + x * scale + dx;
                    int sy = pos_y + y * scale + dy;

                    if (sx >= 0 && sx < width && sy >= 0 && sy < height) {
                        pixels[sy * width + sx] = pixelColor;
                    }
                }
            }
        }
    }
}

void View::freeze() {

    frozen = true;
}

int View::getLineHeight() {
    return font_winFreeSystem14x16.height;
}

void View::renderRect(int pos_x, int pos_y, int width, int height, int color) {
    printf("render rect %d %d\n", width, height);
    for (int y = pos_y; y < pos_y + height; ++y) {
        for (int x = pos_x; x < pos_x + width; ++x) {
            pixels[y * this->width  + x] = color;
        }
    }
}

void View::renderIcon(icon icon, int pos_x, int pos_y, float scale, int color, int background) {
    unsigned int* iconBase = GET_ICON(icon);

    if (pos_x == CENTER) pos_x = width / 2 - ICON_SIZE * scale / 2;

    for (int y = pos_y; y < pos_y + ICON_SIZE * scale; ++y) {
        for (int x = pos_x; x < pos_x + ICON_SIZE * scale; ++x) {
            int fill = iconBase[(int)((y - pos_y) * (1/scale))] >> ((int)(ICON_SIZE - (x - pos_x) * (1/scale))) & 1;
            pixels[y * width  + x] = fill ? color : background;
        }
     }
    icons[icon];
}

int View::getCharPixel(char c, int x, int y, int color, int background) {
    uint16_t row = font_winFreeSystem14x16.bits[16 * (c - 0x20) + y];
    return (0x1 & (row >> (16 - x))) ? color : background;
}

int View::getWordPixel(char* str, int x, int y, int color, int background) {
    return getCharPixel(str[x / 10], x % 10, y, color, background);
}

int View::getWidth() {
    return width;
}

int View::getHeight() {
    return height;
}