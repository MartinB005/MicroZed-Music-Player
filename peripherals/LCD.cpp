#include <stdlib.h>
#include <stdio.h>
#include "LCD.h"
#include "ui/colors.h"
#include "core/mzapo_parlcd.h"
#include "core/mzapo_phys.h"
#include "core/mzapo_regs.h"
#include "string.h"
#include "ui/font_types.h"
#include <functional>

static unsigned char *parlcd_mem_base;
static unsigned int pixels[HEIGHT * WIDTH] = {0x00000000};

void LCD::setView(View* view) {
    view->setWriteDest(pixels, std::bind(&LCD::update, this));
    currentView = view;
}

View* LCD::getCurrentView() {
    return currentView;
}

void LCD::init() {
    parlcd_mem_base = (unsigned char *)map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
    if (parlcd_mem_base == NULL)
        exit(1);
    parlcd_hx8357_init(parlcd_mem_base);
}

void LCD::whiteScreen() {
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        pixels[i] = WHITE;
    }
}

void LCD::update() {
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        parlcd_write_data(parlcd_mem_base, pixels[i]);
    }
}
