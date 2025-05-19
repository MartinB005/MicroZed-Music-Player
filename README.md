# DIY Music Player using MicroZed

This is low-level music player written in C/C++ with custom DA Converter as an audio output. It includes simple GUI with file browser and playing controls, 
which is rendering on 320 x 480 LCD. The Music Player can be controlled by rotary encoders. Current audio level is shown by 32-LED line. See full code documentation [here](https://martinb005.github.io/MicroZed-Music-Player/annotated.html).

## Target Device
- MICROZED EVALUATION KIT ADSAES-Z7MB-7Z010-G
- CPU: Dual ARM® Cortex™-A9 MPCore™ @ 866 MHz (NEON™ & Single / Double Precision Floating Point)
- Operating system: GNU/Linux
- Development kit MicroZed APO (https://gitlab.com/pikron/projects/mz_apo/microzed_apo)

## Supported Audio Formats
- 16-bit PCM WAV, stereo (2-channel), uncompressed
- MPEG-1 Layer III (MP3), stereo, lossy compression

## DA Converter
- 10-bit R2R digital to analog converter with LM386 amplifier
- 2x 74HC595 shift register
- Jack 3.5 mm and terminal block for speaker
- See [dac.sch](dac-design/dac.sch) and [dac.brd](dac-design/dac.brd)

Current implementation uses 3 GPIO pins and 2 shift registers (see [DAC.cpp](peripherals/DAC.cpp) and [mzapo_regs.h](core/mzapo_regs.h) where are adresses of mapped pins).
It is possible to do it without shift registers, if you have 10 free GPIO-s.

## Instalation
```bash
# clone the repo
git clone https://github.com/MartinB005/MicroZed-Music-Player.git

# Navigate to the directory
cd MicroZed-Music-Player

# Edit your local IP address in Makefile
TARGET_IP ?= xxx.xxx.xxx.xxx

# compile and run the program
make run
```

## Used Libraries
MP3 decoding:
https://github.com/lieff/minimp3

## Demonstration Videos
https://github.com/user-attachments/assets/9bd06672-fdcf-4da8-a37d-1d057ed24d74

https://github.com/user-attachments/assets/56f21344-d6c7-4b53-9a67-5a15862af7fc
