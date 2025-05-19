#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>

#include "peripherals/Encoders.h"
#include "ui/Application.h"

int main(int argc, char *argv[]) {
  unsigned char *mem_base;
  uint32_t val_line=5;
  int i,j,k;
  unsigned int c;

  sleep(1);


  Encoders encoders;
  encoders.init();


  // Start the UI
  Application application;
  application.start();

  // Set actions after ecnoders rotation is changed 
  encoders.setOnValueChange(RED_KNOB, [&application] (bool clockwise) {
    MediaPlayer* mediaPlayer = application.getMediaPlayer();

    double change = clockwise ? 0.02 : -0.02;
    double newVol = application.getVolume() + change;
    if (newVol < 0) newVol = 0;
    else if (newVol > 1) newVol = 1;

    printf("volume %.2f\n", newVol);
    application.setVolume(newVol);
  });

  encoders.setOnValueChange(GREEN_KNOB, [&application] (bool clockwise) {
      ListView* list = application.getMainListView();
      application.getLCD()->setView(list);
      clockwise ? list->selectNext() : list->selectPrevious();
  });
  

  encoders.setOnValueChange(BLUE_KNOB, [&application] (bool clockwise) {
   
  });
  
  // Set actions after ecnoders are pressed 
  encoders.setOnPress(RED_KNOB, [&application] () {
    if (application.showingPlayingView()) {
      application.getMediaPlayer()->previousTrack();
    }
    application.showPlayingView();
  });

 encoders.setOnPress(GREEN_KNOB, [&application] () {
    if (application.showingPlayingView()) {
      if (application.isPlayingTrack()) {
        application.getMediaPlayer()->pause();
      }
      else if (application.hasPausedTrack()) {
        application.getMediaPlayer()->play();
      }
      application.showPlayingView();
    
    } else {
      application.openSelectedFile();
      usleep(100000);
    } 
  });

encoders.setOnPress(BLUE_KNOB, [&application] () {
    if (application.showingPlayingView()) {
      application.getMediaPlayer()->nextTrack();
    }

    application.showPlayingView();

  });


  // Constatnly check encoders
  while (true) {
    if (application.isPlayingTrack()) usleep(5000); 
    encoders.check();
  }

  return 0;
}