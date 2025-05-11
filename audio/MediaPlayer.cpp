#include <string.h>
#include <unistd.h>
#include "MediaPlayer.h"


MediaPlayer::MediaPlayer(char* filename) {
    file = fopen(filename, "rb");
    
    if (file != NULL) {
        readWavHeader(file);
    
    } else printf("File %s not found!\n", filename);
}

void MediaPlayer::play() {
    printf("play\n");
    playing = true;
    double interval = 1000000.0 / sampleRate;

    printf("interval: %.3f\n", interval);
    fread(playerBuff, BUFF_SIZE, 1, file);
    fread(fileBuff, BUFF_SIZE, 1, file);
    

    executor.schedule([this]() {
            
        if (readStorage.load()) {
            printf("read from storage...\n");
            std::swap(playerBuff, fileBuff);
            fread(fileBuff, BUFF_SIZE, 1, file);
            readStorage.store(false);
            printf("read completed...\n");
        }

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
            printf("waiting for data\n");
            pos = 0;
            readStorage.store(true);
           // usleep(2);
        }

    }, interval);
}

void MediaPlayer::showAudioLevel(VUMeter* vuMeter) {
    this->vuMeter = vuMeter;
}

void MediaPlayer::stop() {
    executor.stopAll();
    playing = false;
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


void MediaPlayer::readWavHeader(FILE *file) {
    RIFFHeader riffHeader;
    fread(&riffHeader, sizeof(RIFFHeader), 1, file);
    printf("RIFF Chunk ID: %.4s\n", riffHeader.chunkID);
    printf("RIFF Chunk Size: %u\n", riffHeader.chunkSize);
    printf("WAVE Format: %.4s\n", riffHeader.format);

    FmtChunk fmtChunk;
    fread(&fmtChunk, sizeof(FmtChunk), 1, file);
    printf("Subchunk1 ID (fmt): %.4s\n", fmtChunk.subchunk1ID);
    printf("Subchunk1 Size: %u\n", fmtChunk.subchunk1Size);
    printf("Audio Format: %u\n", fmtChunk.audioFormat);
    printf("Channels: %u\n", fmtChunk.numChannels);
    printf("Sample Rate: %u\n", fmtChunk.sampleRate);
    printf("Byte Rate: %u\n", fmtChunk.byteRate);
    printf("Block Align: %u\n", fmtChunk.blockAlign);
    printf("Bits per Sample: %u\n", fmtChunk.bitsPerSample);
    sampleRate = fmtChunk.sampleRate;

    while (true) {
        char chunkID[4];
        uint32_t chunkSize;

        fread(chunkID, sizeof(chunkID), 1, file);
        fread(&chunkSize, sizeof(chunkSize), 1, file);
        printf("Next Chunk ID: %.4s\n", chunkID);
        printf("size: %d\n", chunkSize);

        if (strncmp(chunkID, "data", 4) == 0) {
           // fread(&dataChunk, sizeof(DataChunk), 1, file);
            printf("Subchunk2 ID (data): %.4s\n",  chunkID);
            printf("Subchunk2 Size (audio data): %u\n", chunkSize);
            break;

        } else {
            fseek(file, chunkSize, SEEK_CUR);
        }
    }
}
