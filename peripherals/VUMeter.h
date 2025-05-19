#pragma once
#include <stdint.h>

#define LED_COUNT 32


class VUMeter {

    public:

        /**
         * @brief Map the base memory to control the LED line
         */
        void init();

        /**
         * @brief Set the Max Val for the VU Meter
         *
         * @param val max value
         */
        void setMaxVal(uint16_t val);

        /**
         * @brief 
         * 
         * @param val write new value to VU Meter
         */
        void update(uint16_t val);


    private:
        unsigned char* mem_base;
        uint16_t maxVal;
};