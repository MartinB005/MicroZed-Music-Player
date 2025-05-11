#pragma once
#include <vector>
#include "peripherals/LCD.h"
#include "View.h"

struct Item {
    const char* text;
    int icon;
};

class ListView : public View {

    public:

        ListView(int width, int height);
        void addItem(const char* text, int icon);
        void selectNext();
        void selectPrevious();
        Item getSelectedItem();
        int getSelectedIndex();

    private:
        int selected = 0;
        std::vector<Item> items;

        void update();
};