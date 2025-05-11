#include <string.h>
#include <unistd.h>
#include "MediaPlayer.h"

#define MINIMP3_IMPLEMENTATION
#include "minimp3_ex.h"


MediaPlayer::MediaPlayer(char* filename) {
    if (strstr(filename, ".wav")) {
        audioFormat = WAV;
        file = fopen(filename, "rb");
   
        if (file != NULL) {
            printf("File %s not found!\n", filename);
            return;
        }

        readWavHeader(file);
    } 

    else if (strstr(filename, ".mp3")) {
        audioFormat = MP3;

        if (mp3dec_ex_open(&dec, filename, MP3D_SEEK_TO_SAMPLE)) {
            printf("Failed to open MP3 file\n");
            mp3dec_ex_close(&dec);
            return;
        }
    }
}

void MediaPlayer::play() {
    printf("play\n");
    playing = true;
    double interval = 1000000.0 / sampleRate;

    prepareBuffers();

    executor.schedule([this]() {
        if (readStorage.load()) swapAndFeed();
    }, 50);

    int pos = 0;
    executor.schedule([this, &pos]() {
        int16_t sample = playerBuff[pos];
        uint8_t voltage = (uint32_t)(sample + 32768) * 1024.0 / 65535.0 * volume;
        dac->write(voltage);
        dac->latch();
        vuMeter->update(sample * volume);
        pos += 2;

        if (pos >= BUFF_SIZE / 2) {
            pos = 0;
            readStorage.store(true);
        }

    }, interval);
}

void MediaPlayer::showAudioLevel(VUMeter* vuMeter) {
    this->vuMeter = vuMeter;
}

void MediaPlayer::stop() {
    executor.stopAll();
    playing = false;
    
    switch (audioFormat) {
        case WAV:
            fclose(file);
            break;
        
        case MP3:
            mp3dec_ex_close(&dec);
    }
}

void MediaPlayer::setVolume(double volume) {
    this->volume = volume;
}

double MediaPlayer::getVolume() {
    return volume;
}


void MediaPlayer::setOutput(DAC* dac) {
    this->dac = dac;
}

bool MediaPlayer::isPlaying() {
    return playing;
}

void MediaPlayer::prepareBuffers() {
    switch (audioFormat) {
        case WAV:
            fread(playerBuff, BUFF_SIZE, 1, file);
            fread(fileBuff, BUFF_SIZE, 1, file);
            break;
        
        case MP3:
            mp3dec_ex_read(&dec, playerBuff, BUFF_SIZE);
            mp3dec_ex_read(&dec, fileBuff, BUFF_SIZE);
    }
}

void MediaPlayer::swapAndFeed() {
    printf("read from storage...\n");
    std::swap(playerBuff, fileBuff);

     switch (audioFormat) {
        case WAV:
            fread(fileBuff, BUFF_SIZE, 1, file);
            break;
        case MP3:
            mp3dec_ex_read(&dec, fileBuff, BUFF_SIZE);
    }

    readStorage.store(false);
}

void MediaPlayer::readWavHeader(FILE *file) {
    RIFFHeader riffHeader;
    fread(&riffHeader, sizeof(RIFFHeader), 1, file);
    printf("RIFF Chunk ID: %.4s\n", riffHeader.chunkID);
    printf("RIFF Chunk Size: %u\n", riffHeader.chunkSize);
    printf("WAVE Format: %.4s\n", riffHeader.format);

    FmtChunk fmtChunk;
    fread(&fmtChunk, sizeof(FmtChunk), 1, file);
    printf("Audio Format: %u\n", fmtChunk.audioFormat);
    printf("Channels: %u\n", fmtChunk.numChannels);
    printf("Sample Rate: %u\n", fmtChunk.sampleRate);
    sampleRate = fmtChunk.sampleRate;

    while (true) {
        char chunkID[4];
        uint32_t chunkSize;

        fread(chunkID, sizeof(chunkID), 1, file);
        fread(&chunkSize, sizeof(chunkSize), 1, file);
        printf("Next Chunk ID: %.4s\n", chunkID);
        printf("size: %d\n", chunkSize);

        if (strncmp(chunkID, "data", 4) == 0) {
            printf("Subchunk2 ID (data): %.4s\n",  chunkID);
            printf("Subchunk2 Size (audio data): %u\n", chunkSize);
            break;

        } else {
            fseek(file, chunkSize, SEEK_CUR);
        }
    }
}
