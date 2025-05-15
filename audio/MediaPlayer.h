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
        MediaPlayer(std::string filename);
        MediaPlayer(std::vector<std::string> playlist);
        void setOutput(DAC* dac);
        void showAudioLevel(VUMeter* vuMeter);
        void setVolume(double volume);
        void setOnPositionUpdate(std::function<void(int)> func);
        double getVolume();
        bool isPlaying();
        std::string getTrackName();
        void play();
        void play(int trackNumber);
        void nextTrack();
        void previousTrack();
        void pause();
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

        void readWavHeader(FILE* file);
        void prepareBuffers();
        void swapAndFeed();
        void prepareTrack(std::string filename);
        
};