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

        /**
         * @brief start the user interface of the application
         */
        void start();

        bool isPlayingTrack();
        bool hasPausedTrack();

        /**
         * @brief Set the global volume for the application and show the volume view
         * 
         * @param volume new volume
         */
        void setVolume(double volume);
        double getVolume();

        /**
         * @return true if playing view is shown
         * @return false  if playing view is not shown
         */
        bool showingPlayingView();

        /**
         * @brief Get the main list object of the application 
         * 
         * @return ListView* list pointer
         */
        ListView* getMainListView();

        /**
         * @brief Get the Media Player object of the application
         * 
         * @return MediaPlayer* media player pointer
         */
        MediaPlayer* getMediaPlayer();

        /**
         * @brief Get the LCD object of the application
         * 
         * @return LCD* lcd pointer
         */
        LCD* getLCD();

        /**
         * @brief Open the file which is currently selected in main listview
         */
        void openSelectedFile();

        /**
         * @brief Show the currently playing track view
         */
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

        /**
         * @brief Open directory with the specified name
         * 
         * @param name directory
         */
        void openDir(std::string name);

        /**
         * @brief Open file with the specified name
         * 
         * @param name filename
         */
        void openFile(std::vector<File>& files, int index);
};