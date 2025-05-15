#include <stdlib.h>
#include <stdio.h>
#include "ListView.h"
#include "colors.h"

#define DIVIDER_HEIGHT 2
#define LINE_SPACING DIVIDER_HEIGHT + 2
#define LEFT_PADDING 10

ListView::ListView(int width, int height) : View(width, height) {
    
}

void ListView::addItem(std::string text, int icon) {
    Item item = {text, icon};
    items.push_back(item);
    selected = 0;
}

void ListView::update()  {
    printf("update list\n");
    View::blackScreen();

    int item_height = getLineHeight() + LINE_SPACING;
    
    for (int i = scroll, x = 0; i < items.size() &&  x < height; i++, x += item_height) {
        renderText(items[i].text.c_str(), LEFT_PADDING, x, selected == i ? BLACK : WHITE, selected == i ? WHITE : BLACK);
       // renderRect(0, i * item_height - DIVIDER_HEIGHT, WIDTH, DIVIDER_HEIGHT, WHITE);
    }

    View::updateScreen();
}

void ListView::clear() {
    items.clear();
}

void ListView::selectNext() {
    if (selected < items.size() - 1) {
        selected++;

        int item_height = getLineHeight() + LINE_SPACING;
        if ((selected - scroll + 1) * item_height > height) {
            scroll++;
        }
        update();
    }
}

void ListView::selectPrevious() {
    if (selected > 0) {
        selected--;

        int item_height = getLineHeight() + LINE_SPACING;
        if ((selected - scroll) * item_height < 0) {
            scroll--;
        }
        
        update();
    }
}

Item ListView::getSelectedItem() {
    return items[selected];
}

int ListView::getSelectedIndex() {
    return selected;
}


