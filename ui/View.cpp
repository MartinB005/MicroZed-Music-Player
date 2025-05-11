#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "colors.h"
#include "font_types.h"
#include "View.h"

View::View(int width, int height) {
    this->width = width;
    this->height = height;
}


void View::setWriteDest(unsigned int* addr, std::function<void()> func) {
    pixels = addr;
    this->onUpdate = func;

    printf("write dest %p\n", pixels);
}

unsigned int* View::getPixels() {
    return pixels;
}

void View::update() {
    if (pixels == nullptr) {
        pixels = (unsigned int*) malloc(width * height * sizeof(unsigned int));
    }

    if (!frozen) onUpdate();
}

void View::renderText(char* text, int pos_x, int pos_y, int color, int background) {

    for (int y = pos_y; y < pos_y + font_winFreeSystem14x16.height; y++) {
        for (int x = pos_x; x < pos_x + strlen(text) * 10; x++) {
            pixels[y * width + x] = getWordPixel(text, x - pos_x, y - pos_y, color, background);
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
  //  printf("render rect\n");
    for (int y = pos_y; y < pos_y + height; y++) {
        for (int x = pos_x; x < pos_x + width; x++) {
            pixels[y * width  + x] = color;
        }
    }
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