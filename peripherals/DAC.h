#pragma once
#include <stdint.h>


class DAC {

    public:
        void init();
        void write(uint16_t val);
        void latch();
    
    private:
        unsigned char* pmod1;
        unsigned char* pmod2;
        bool ready;
};