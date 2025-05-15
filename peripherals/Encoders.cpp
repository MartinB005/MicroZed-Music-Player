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

    if (abs(red - lastVals[RED_KNOB]) >= STEP) {
        onValChange[RED_KNOB](red < lastVals[RED_KNOB]);
        lastVals[RED_KNOB] = red;
    } 
    if (abs(green - lastVals[GREEN_KNOB]) >= 4) {
        onValChange[GREEN_KNOB](green < lastVals[GREEN_KNOB]);
        lastVals[GREEN_KNOB] = green;
    } 
    if (abs(blue - lastVals[BLUE_KNOB]) >= 4) {
        onValChange[BLUE_KNOB](blue < lastVals[BLUE_KNOB]);
        lastVals[BLUE_KNOB] = blue;
    }



   // printf("%X %d %d %d\n", val, red, green, blue);
}

