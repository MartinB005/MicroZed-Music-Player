#pragma once
#include <stdint.h>
#include <stdio.h>
#include <atomic>
#include "peripherals/VUMeter.h"
#include "peripherals/DAC.h"
#include "utils/Executor.h"
#include "wav.h"

#define BUFF_SIZE 65536

class MediaPlayer {
    
    public:
        MediaPlayer(char* filename);
        void setOutput(DAC* dac);
        void showAudioLevel(VUMeter* vuMeter);
        void setVolume(double volume);
        double getVolume();
        bool isPlaying();
        void play();
        void pause();
        void stop();
        
    private:
        int16_t bufferA[BUFF_SIZE];
        int16_t bufferB[BUFF_SIZE];

        int16_t* playerBuff = bufferA;
        int16_t* fileBuff = bufferB;
        uint32_t sampleRate;
        
        Executor executor;
        VUMeter* vuMeter;
        FILE* file;
        DAC* dac;
        std::atomic<bool> readStorage{false};
        int playPos = 0;
        double volume = 0.4;
        bool playing = false;

        void readWavHeader(FILE* file);
        
};