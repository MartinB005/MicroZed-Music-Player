#pragma once
#include "View.h"

class VolumeView : public View {

    public:

        /**
         * @brief Construct a new Volume View object
         */
        VolumeView();

        /**
         * @brief Set the new value of the volume
         * @param volume 
         */
        void setValue(double volume);

        /**
         * @brief Update the view
         */
        void update() override; 

};