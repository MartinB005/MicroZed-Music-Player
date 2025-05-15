#pragma once
#include "View.h"

class VolumeView : public View {

    public:
        VolumeView();
        void setValue(double volume);
        void update() override; 

};