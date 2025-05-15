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

        void init();
        void setOnValueChange(Encoder encoder, std::function<void(bool)>);
        void setOnPress(Encoder encoder,  std::function<void()>);
        void check();

    private:
        unsigned char *mem_base;
        uint8_t lastVals[3];
        bool pressed[3] = {false};
        std::function<void(bool)> onValChange[3];
        std::function<void()> onPress[3];
};