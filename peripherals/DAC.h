#pragma once
#include <stdint.h>


class DAC {

    public:

        /**
         * @brief map the base memory of GPIO pins to pointers
         */
        void init();

        /**
         * @brief write immediate value to DA converter via shift registers
         * @param value - value between 0 and 2 ^ (DAC_RESOLUTION - OFFSET)
        */
        void write(uint16_t val);

        /**
         * @brief applies written value to the output
        */
        void latch();
    
    private:
        unsigned char* pmod1;
        unsigned char* pmod2;
        bool ready;
};