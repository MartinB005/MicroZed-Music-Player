#include <stdlib.h>
#include <stdio.h>
#include "core/mzapo_parlcd.h"
#include "core/mzapo_phys.h"
#include "core/mzapo_regs.h"
#include "Encoders.h"


#define STEP 4

void Encoders::init() {
    mem_base = (unsigned char*) map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
}

void Encoders::setOnValueChange(Encoder encoder, std::function<void(bool)> func) {
    this->onValChange[encoder] = func;
}

void Encoders::setOnPress(Encoder encoder, std::function<void()> func) {
    this->onPress[encoder] = func;
}

void Encoders::check() {
    uint32_t val = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);


    for (int i = 0; i < 3; i++) {
        if (!pressed[i] && ((val >> (26 - i)) & 1)) onPress[i]();
        pressed[i] = (val >> (26 - i)) & 1;
    }

    uint8_t red = (val >> 16) & 0xFF; 
    uint8_t green = (val >> 8) & 0xFF;
    uint8_t blue = val & 0xFF;

    if (red != lastVals[RED]) onValChange[RED](red < lastVals[RED]);
    if (green != lastVals[GREEN]) onValChange[GREEN](green < lastVals[GREEN]);
    if (blue != lastVals[BLUE]) onValChange[BLUE](blue < lastVals[BLUE]);

    lastVals[RED] = red;
    lastVals[GREEN] = green;
    lastVals[BLUE] = blue;



   // printf("%X %d %d %d\n", val, red, green, blue);
}

