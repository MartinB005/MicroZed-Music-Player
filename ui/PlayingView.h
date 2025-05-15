#pragma once
#include "View.h"
#include "peripherals/LCD.h"
#include "audio/MediaPlayer.h"

class PlayingView : public View  {

    public:
        PlayingView();
        void update() override;
        void setMediaPlayer(MediaPlayer*);
        void updateSeekBar(int position);

    private:
        MediaPlayer* mediaPlayer;
};