#pragma once
#include <vector>
#include "LCD.h"
#include "View.h"

struct Item {
    char* text;
    int icon;
};

class ListView : public View {

    public:

        ListView(int width, int height);
        void addItem(char* text, int icon);
        void selectNext();
        void selectPrevious();
        Item getSelectedItem();

    private:
        int selected = 0;
        std::vector<Item> items;

        void update();
};