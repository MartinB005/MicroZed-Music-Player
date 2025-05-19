# Technical Documentation

## Music Player Specifications

### Audio File Format
- 16-bit PCM WAV, stereo (2-channel), uncompressed
- MPEG-1 Layer III (MP3), stereo, lossy compression

### Audio Output
- 10-bit PCM mono audio generated via an R-2R resistor ladder DAC
- digital input provided through 74HC595 shift registers
- used LM386 amplifier to boost the signal
- analog output available via jack 3.5 mm & terminal block for speaker

## Project Structure
Project is written in C/C++ and object-oriented. Its code is organized into several folders.
- ```main.cpp``` - entry point to zhe application, handles encoders actions

### ```audio/```
- contains classes for core audio handling
- ```MediaPlayer.cpp``` - manages audio stream processing, buffering, and playback
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
- contains classes and files of UI compontents and fonts
- ```Application.cpp``` - entry point to UI, contains methods for opeing files and folders
- ```View.cpp``` - parent of each UI component, contains methods for rendering text and graphics
- ```ListView.cpp``` - UI component for rendering list of items
- ```PlayingView.cpp``` - UI component for rendering name, controls and seek bar of currently playing track
- ```VolumeView.cpp``` - UI component for rendering volume window on its change

### ```utils/```
- contains util classes for the music player
- ```Executors.cpp``` - class for scheduling actions and running them on different threads
- ```FileManager.cpp``` - manages files and folders of the device



## PCB fabrication
- done at home
- only basic components and tools were used
- designed using Autodesk Eagle software
- paths were transfered using laser printer and iron
- etched using FeCl3 (ferric chloride)
- standard procedures were followed (drill holes and soldering components)
