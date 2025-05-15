#include <stdlib.h>
#include <stdio.h>
#include "peripherals/LCD.h"
#include "peripherals/DAC.h"
#include "peripherals/VUMeter.h"
#include "utils/FileManager.h"
#include "VolumeView.h"
#include "PlayingView.h"
#include "ListView.h"

class Application {
    
    public:
        Application();
        void start();
        bool isPlayingTrack();
        bool hasPausedTrack();
        void setVolume(double volume);
        double getVolume();
        bool showingPlayingView();
        ListView* getMainListView();
        MediaPlayer* getMediaPlayer();
        LCD* getLCD();
        void openSelectedFile();
        void showPlayingView();


    private:
        MediaPlayer* mediaPlayer;
        FileManager fileManager;
        ListView list;
        VUMeter vuMeter;
        PlayingView playingView;
        VolumeView volumeView;
        DAC dac;
        LCD lcd;
        double volume = 0.5;

        void openDir(std::string name);
        void openFile(std::vector<File>& files, int index);
};