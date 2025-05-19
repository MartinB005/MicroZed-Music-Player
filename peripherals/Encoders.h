#pragma once
#include <functional>
#include <stdint.h>

enum Encoder {
    RED_KNOB,
    GREEN_KNOB,
    BLUE_KNOB
};

class Encoders {

    public:

        /**
         * @brief map the base memory of the encoders input
         */
        void init();

        /**
         * @brief Set the callback when rotation is changed
         * 
         * @param encoder which (RED, GREN, BLUE) 
         * @param std::function callback
         */
        void setOnValueChange(Encoder encoder, std::function<void(bool)>);

         /**
         * @brief Set the callback when encoder is pressed
         * 
         * @param encoder which (RED, GREN, BLUE) 
         * @param std::function callback
         */
        void setOnPress(Encoder encoder,  std::function<void()>);

        /**
         * @brief update value from input
         */
        void check();

    private:
        unsigned char *mem_base;
        uint8_t lastVals[3];
        bool pressed[3] = {false};
        std::function<void(bool)> onValChange[3];
        std::function<void()> onPress[3];
};