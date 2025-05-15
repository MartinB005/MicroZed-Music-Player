#include <stdlib.h>
#include "core/mzapo_phys.h"
#include "core/mzapo_regs.h"
#include "VUMeter.h"

#define LED_LINE *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o)
#define AMPLIFY 8

void VUMeter::init() {
    mem_base = (unsigned char*) map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

    if (mem_base == NULL)
        exit(1);
}

void VUMeter::setMaxVal(uint16_t val) {
    this->maxVal = val;
}

void VUMeter::update(uint16_t level) {
    uint8_t val = level * AMPLIFY * LED_COUNT / maxVal;
    LED_LINE = 0xFFFFFFFF >> (LED_COUNT - val);
}