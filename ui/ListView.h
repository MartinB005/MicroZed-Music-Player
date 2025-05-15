#pragma once
#include <vector>
#include <string>
#include "peripherals/LCD.h"
#include "View.h"

struct Item {
    std::string text;
    int icon;
};

class ListView : public View {

    public:

        ListView(int width, int height);
        void addItem(std::string, int icon);
        void selectNext();
        void selectPrevious();
        Item getSelectedItem();
        int getSelectedIndex();
        void update() override;
        void clear();

    private:
        int selected = 0;
        int scroll = 0;
        std::vector<Item> items;
};