#pragma once
#include <stdint.h>

#define LED_COUNT 32

class VUMeter {

    public:

        void init();
        void setMaxVal(uint16_t val);
        void update(uint16_t val);


    private:
        unsigned char* mem_base;
        uint16_t maxVal;
};