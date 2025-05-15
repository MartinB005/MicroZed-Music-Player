#include <stdio.h>
#include <string.h>
#include "PlayingView.h"

#define CONTROLS_Y 180
#define CONTROLS_SCALE 2.0
#define TRACK_NAME_Y 50
#define SEEK_BAR_Y 130
#define TEXT_SCALE 2
#define PADDING 20

PlayingView::PlayingView() : View(WIDTH, HEIGHT) {

}

void PlayingView::setMediaPlayer(MediaPlayer* mediaPlayer) {
    this->mediaPlayer = mediaPlayer;
}

void PlayingView::update() {
    printf("show\n");
    View::blackScreen();
    int sc = CONTROLS_SCALE;
    
    if (mediaPlayer != NULL) {
        std::string trackName = mediaPlayer->getTrackName();
        if (trackName.length() > 20) trackName = trackName.substr(0, 20) + "..";
        renderText(trackName.c_str(), CENTER, TRACK_NAME_Y, WHITE, BLACK, TEXT_SCALE);
    }

    icon mid_icon = mediaPlayer != NULL && mediaPlayer->isPlaying() ? PAUSE : PLAY;

    renderIcon(PREVIOUS, WIDTH_RATIO(30, sc), CONTROLS_Y, sc, WHITE);
    renderIcon(mid_icon, WIDTH_RATIO(50, sc), CONTROLS_Y, sc, WHITE);
    renderIcon(SKIP, WIDTH_RATIO(65, sc), CONTROLS_Y, sc, WHITE);

    View::updateScreen();
}

void PlayingView::updateSeekBar(int position) {
    if (position > 0) {
        renderRect(PADDING, SEEK_BAR_Y, position * (WIDTH - PADDING * 2) / 100, 5, BLUE);
        View::updateScreen();
    }
}