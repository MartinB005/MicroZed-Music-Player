# Technical Documentation

## Music Player Specifications
- 16-bit PCM WAV, stereo (2-channel), uncompressed
- MPEG-1 Layer III (MP3), stereo, lossy compression

## Audio Output
- 10-bit PCM mono using R2R DA Converter

## Project Structure
Project is written in C/C++ and object-oriented. Its code is organized into several folders.
 ```main.cpp``` - entry point to application, handles encoders actions

### ```audio/```
- contains classes for core audio handling
- ```MediaPlayer.cpp``` - manages audio stream processing, buffering, and playback.
- ```wav.h```, ```minimp3.h``` - process audio formats

### ```core/```
- contains files for managing memory adresses and registers

### ```dac-design/```
- contains files for PCB design of DA Converter

### ```peripherals/```
- contains classes for controlling peripheral devices
- ```LCD.cpp``` - controls display and manage views
- ```DAC.cpp``` - provides writing to DA converter
- ```VUMeter.cpp``` - controls the 32 LED line visual output
- ```Encoders.cpp``` - checks states of the encoders and handles their changes

### ```ui/```
- contains classes and files of ui compontents and fonts
- ```Application.cpp``` - entry point to UI, contains methods for opeing files and folders
- ```View.cpp``` - parent of each UI component, contains methods for rendering text and graphics
- ```ListView.cpp``` - UI component for rendering list of items
- ```PlayingView.cpp``` - UI component for rendering name, controls and seek bar of currently playing track
- ```VolumeView.cpp``` - UI component for rendering volume window on its change

### ```utils/```
- contains util classes for the music player
- ```Executors.cpp``` - class for scheduling actions and running them on different threads
- ```FileManager.cpp``` - manages files and folders of the device
