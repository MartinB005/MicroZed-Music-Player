#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <atomic>
#include <vector>
#include <functional>
#include "peripherals/VUMeter.h"
#include "peripherals/DAC.h"
#include "utils/Executor.h"
#include "wav.h"


extern "C" {
#include "minimp3_ex.h"
}


#define BUFF_SIZE 65536

enum AudioFormat {
    WAV, MP3
};

class MediaPlayer {
    
    public:

        /**
         * @brief Construct a new Media Player object
         * Prepares the Media Player to play a track
         * 
         * @param filename which file should be played
         * 
         * Supported formats: MPEG Layer III (MP3) and PCM 16-bit 2-channel WAV
         */
        MediaPlayer(std::string filename);

        /**
         * @brief Construct a new Media Player object
         * Prepares the Media Player to play a list of tracks
         * 
         * @param playlist list of filenames
         * 
         * Supported formats: MPEG Layer III (MP3) and PCM 16-bit 2-channel WAV
         */
        MediaPlayer(std::vector<std::string> playlist);

        /**
         * @brief Set the output for the media MediaPlayer
         * The MediaPlayer will write audio samples to this object
         * 
         * @param dac DA Converter object for generating audio output
         */
        void setOutput(DAC* dac);

        /**
         * @brief Set the visual output for the MediaPlayer
         * The MediaPlayer will update the output on each sample
         * 
         * @param vuMeter 
         */
        void showAudioLevel(VUMeter* vuMeter);

        /**
         * @brief Set the volume of the MediaPlayer
         * 
         * @param volume value between 0 and 1
         */
        void setVolume(double volume);

        /**
         * @brief Set the callback when track position is changed
         * Useful for seekbars and elapsed time view
         * 
         * @param func callback
         */
        void setOnPositionUpdate(std::function<void(int)> func);

        double getVolume();
        bool isPlaying();

        /**
         * @brief Get the Track Name object
         * 
         * @return std::string the name of the currently playing track
         */
        std::string getTrackName();

        /**
         * @brief start playing the prepared track
         */
        void play();

        /**
         * @brief start playing the track on certain position in the playlist
         * 
         * @param trackNumber the index of the track in the playlist
         */
        void play(int trackNumber);

        /**
         * @brief stop the current track and start playing the next track in the playlist
         */
        void nextTrack();

        /**
         * @brief stop the current track and start playing the previous track in the playlist
         */
        void previousTrack();

        /**
         * @brief pause the current playing track
         */
        void pause();

        /**
         * @brief stop the current playing track and terminate all MediaPlayer executing actions
         */
        void stop();
        
    private:
        int16_t bufferA[BUFF_SIZE];
        int16_t bufferB[BUFF_SIZE];

        int16_t* playerBuff = bufferA;
        int16_t* fileBuff = bufferB;
        uint32_t sampleRate;
        AudioFormat audioFormat;
        
        Executor executor;
        VUMeter* vuMeter;
        FILE* file;
        mp3dec_ex_t dec;
        DAC* dac;
        std::function<void(int)> onPosUpdate = NULL;
        std::string trackName;
        std::atomic<bool> readStorage{false};
        std::vector<std::string> playlist;
        unsigned long numSamples;
        unsigned int position = 0;
        double volume = 0.5;
        bool playing = false;
        bool prepared = false;
        int trackNumber = 0;
        int buffPos;

        /**
         * @brief read a header from a WAVE file
         * 
         * @param file to read
         */
        void readWavHeader(FILE* file);

        /**
         * @brief write first N bytes to buffers from the file according to BUFF_SIZE 
         */
        void prepareBuffers();

        /**
         * @brief swap buffer pointers and read further data from the file         * 
         */
        void swapAndFeed();

        /**
         * @brief check if file exists and read the header of the file according to its format
         * 
         * @param filename file to read
         */
        void prepareTrack(std::string filename);
        
};