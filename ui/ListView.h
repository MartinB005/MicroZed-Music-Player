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
        
        /**
         * @brief Construct a new List View object
         * 
         * @param width of the view
         * @param height of the view
         */
        ListView(int width, int height);

        /**
         * @brief Add new item to the list
         * 
         * @param icon at the start of the item (not implemented yet)
         */
        void addItem(std::string, int icon);

        /**
         * @brief Select next item of the list
         */
        void selectNext();

        /**
         * @brief Select previous item of the list
         */
        void selectPrevious();

        /**
         * @brief Get the selected item struct
         */
        Item getSelectedItem();

        /**
         * @brief Get the Selected Index
         */
        int getSelectedIndex();

        /**
         * @brief Update the view
         */
        void update() override;

        /**
         * @brief Remove all items from list
         */
        void clear();

    private:
        int selected = 0;
        int scroll = 0;
        std::vector<Item> items;
};