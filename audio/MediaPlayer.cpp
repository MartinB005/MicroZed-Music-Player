#include <string.h>
#include <unistd.h>
#include "MediaPlayer.h"

extern "C" {
#define MINIMP3_IMPLEMENTATION
#include "minimp3_ex.h"
}


#define ROOT_VOLUME 0.4

bool hasExtension(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}



MediaPlayer::MediaPlayer(std::string filename) {
   prepareTrack(filename);
}

MediaPlayer::MediaPlayer(std::vector<std::string> playlist) {
    this->playlist = playlist;
}

void MediaPlayer::prepareTrack(std::string filename) {
    trackName = filename;
   
    if (hasExtension(filename, ".wav")) {
        audioFormat = WAV;
        file = fopen(filename.c_str(), "rb");
   
        if (file == NULL) {
            printf("File %s not found!\n", filename);
            return;
        }

        readWavHeader(file);
    } 

    else if (hasExtension(filename, ".mp3")) {
        audioFormat = MP3;

        if (mp3dec_ex_open(&dec, filename.c_str(), MP3D_SEEK_TO_SAMPLE)) {
            printf("Failed to open MP3 file\n");
            mp3dec_ex_close(&dec);
            return;
        }

        numSamples = dec.samples;
        sampleRate = dec.info.hz;
    }
}

void MediaPlayer::play(int trackNumber) {
    stop();
    this->trackNumber = trackNumber;
    //printf("file %s", playlist[trackNumber].c_str());
    prepareTrack(playlist[trackNumber]);
    play();
}

void MediaPlayer::play() {
    playing = true;
    buffPos = 0;
    int test = 0;

    if (!prepared) {
        double interval = 1000000.0 / sampleRate;
        position = 0;

        prepareBuffers();

        executor.schedule([this]() {
            if (readStorage.load()) swapAndFeed();
        }, interval / 1000);

     printf("init\n");
     printf("sample rate: %d\n", sampleRate);

        executor.schedule([this, &test]() {
            if(playing) {
                int16_t sample = playerBuff[buffPos];
                uint8_t voltage = (uint32_t)(sample + 32768) * 1024.0 / 65535.0 * volume * ROOT_VOLUME;

                dac->write(voltage);
                dac->latch();
                vuMeter->update(sample * volume);
                buffPos += 2;
                position += 1;
                
                if (buffPos >= (audioFormat == WAV ? BUFF_SIZE / 2 : BUFF_SIZE)) {
                    buffPos = 0;
                    readStorage.store(true);
                }
            }

        }, interval, true);

        prepared = true;
    }
}

void MediaPlayer::nextTrack() {
    if (trackNumber < playlist.size() - 1) {
        stop();
        play(++trackNumber);
    }
}

void MediaPlayer::previousTrack() {
    if (trackNumber > 0) {
        stop();
        sleep(1);
        play(--trackNumber);
    }
}


void MediaPlayer::pause() {
    playing = false;
}

void MediaPlayer::showAudioLevel(VUMeter* vuMeter) {
    this->vuMeter = vuMeter;
}

std::string MediaPlayer::getTrackName() {
    size_t pos = this->trackName.find_last_of('/');
   
    if (pos != std::string::npos) {
        return this->trackName.substr(pos + 1);
    }

    return this->trackName;
}

void MediaPlayer::stop() {
    executor.stopAll();
    playing = false;

    if (prepared) {
        switch (audioFormat) {
            case WAV:
                fclose(file);
                break;
            
            case MP3:
                mp3dec_ex_close(&dec);
        }
    }
    prepared = false;
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

void MediaPlayer::setOnPositionUpdate(std::function<void(int)> func) {
    this->onPosUpdate = func;
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

    if (position >= numSamples && trackNumber < playlist.size() - 1) {
        play(trackNumber + 1);
    }

    switch (audioFormat) {
        case WAV:
            fread(fileBuff, BUFF_SIZE, 1, file);
            break;
        case MP3:
            mp3dec_ex_read(&dec, fileBuff, BUFF_SIZE);
            break;
    }

    printf("numSamples %d %d %d", numSamples, position, (int) ((float) position / numSamples * 100.0));
    if (onPosUpdate != NULL) onPosUpdate((int) ((float) position / numSamples * 100.0));

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
    numSamples = 0; 

    while (true) {
        char chunkID[4];
        uint32_t chunkSize;

        fread(chunkID, sizeof(chunkID), 1, file);
        fread(&chunkSize, sizeof(chunkSize), 1, file);

        printf("Next Chunk ID: %.4s\n", chunkID);
        printf("size: %d\n", chunkSize);
        numSamples += chunkSize / (fmtChunk.numChannels * fmtChunk.bitsPerSample / 8);

        if (strncmp(chunkID, "data", 4) == 0) {
            printf("Subchunk2 ID (data): %.4s\n",  chunkID);
            printf("Subchunk2 Size (audio data): %u\n", chunkSize);
            break;

        } else {
            fseek(file, chunkSize, SEEK_CUR);
        }
    }
}
