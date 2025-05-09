#include <stdlib.h>
#include "ListView.h"
#include "LCD.h"
#include "colors.h"

#define DIVIDER_HEIGHT 2
#define LINE_SPACING DIVIDER_HEIGHT + 2
#define LEFT_PADDING 10

ListView::ListView(int width, int height) : View(width, height) {
    
}

void ListView::addItem(char* text, int icon) {
    Item item = {text, icon};
    items.push_back(item);
    update();
}

void ListView::update() {
    int item_height = getLineHeight() + LINE_SPACING;
    for (int i = 0; i < items.size() &&  i * item_height < height; i++) {
        renderText(items[i].text, LEFT_PADDING, i * item_height, selected == i ? BLACK : WHITE, selected == i ? WHITE : BLACK);
       // renderRect(0, i * item_height - DIVIDER_HEIGHT, WIDTH, DIVIDER_HEIGHT, WHITE);
    }

   /// renderText(items[0].text, LEFT_PADDING, 0, WHITE, BLACK);
    View::update();
}

void ListView::selectNext() {
    if (selected < items.size() - 1) {
        selected++;
        update();
    }
}

void ListView::selectPrevious() {
    if (selected > 0) {
        selected--;
        update();
    }
}

Item ListView::getSelectedItem() {
    return items[selected];
}

