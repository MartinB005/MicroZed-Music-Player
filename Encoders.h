#pragma once
#include <functional>
#include <stdint.h>

enum Encoder {
    RED,
    GREEN,
    BLUE
};

class Encoders {

    public:

        void init();
        void setOnValueChange(Encoder encoder, std::function<void(bool)>);
        void setOnPress(Encoder encoder, void (*func)());
        void check();

    private:
        unsigned char *mem_base;
        uint8_t lastVals[3];
        bool pressed[3] = {false};
        std::function<void(bool)> onValChange[3];
        void (*onPress[3])();
};