#pragma once
#include "View.h"
#include "peripherals/LCD.h"
#include "audio/MediaPlayer.h"

class PlayingView : public View  {

    public:

        /**
         * @brief Construct a new Playing View object
         */
        PlayingView();

        /**
         * @brief Update the view
         */
        void update() override;

        /**
         * @brief Set the Media Player object for playing view
         */
        void setMediaPlayer(MediaPlayer*);

        /**
         * @brief update the seek bar in playing view
         * 
         * @param position value between 0 and 100
         */
        void updateSeekBar(int position);

    private:
        MediaPlayer* mediaPlayer;
};