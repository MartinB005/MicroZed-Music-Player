#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>

#include "peripherals/Encoders.h"
#include "ui/Application.h"



void test() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  printf("time %ld\n", ts.tv_sec);
}


int main(int argc, char *argv[]) {
  unsigned char *mem_base;
 // unsigned char *parlcd_mem_base;
  uint32_t val_line=5;
  int i,j,k;
  unsigned int c;
  
  printf("Hello world\n");
  

  sleep(1);

  /*
   * Setup memory mapping which provides access to the peripheral
   * registers region of RGB LEDs, knobs and line of yellow LEDs.
   */


  Encoders encoders;
  encoders.init();

  Application application;
  application.start();


  /*while (true)
  {
    for (int i = 0; i < 16; i++) {
    dac.write(1 << i);
    dac.latch();
    printf("%d\n", 1 << i);
    getchar();
    sleep(2);
  }
  }*/
  
  

//list.addItem("test", DIRECTORY);
  

  


 /*mem_base = (unsigned char*) map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  
  uint32_t val = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
  printf("val %d\n", val);*/
  /* Store the read value to the register controlling individual LEDs */
  //*(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = val;
  
 

  
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
   
  //  printf("blue %d\n", clockwise);
  });
  

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
  /*  printf("blue\n");
    DAC dac;
    VUMeter vuMeter;
    MediaPlayer* mediaPlayer = new MediaPlayer("/home/Music/quick.mp3");
    mediaPlayer->setOutput(&dac);
    mediaPlayer->showAudioLevel(&vuMeter);
    mediaPlayer->play();*/
    if (application.showingPlayingView()) {
      application.getMediaPlayer()->nextTrack();
    }

    application.showPlayingView();

  });

  while (true) {
    if (application.isPlayingTrack()) usleep(5000); 
    encoders.check();
  }


 // mem_base = (unsigned char*) map_phys_address(DCSPDRV_REG_BASE_PHYS_0, SPILED_REG_SIZE, 0);

/*
struct timespec interval = {.tv_sec = 0, .tv_nsec = 500000}; // 0.5ms
struct timespec next;
clock_gettime(CLOCK_MONOTONIC, &next);

  bool on = false;
  while (true) {
     uint32_t val = *(volatile uint32_t*)(mem_base + DCSPDRV_REG_CR_o);
     on = !on;
     if (on) val |= (1 << 5);
     else val &= ~(1 << 5);
     *(volatile uint32_t*)(mem_base + DCSPDRV_REG_CR_o) = val;
     //printf("on %d\n", on);
    
    next.tv_nsec += interval.tv_nsec;
    if (next.tv_nsec >= 1000000000) {
      next.tv_sec += 1;
      next.tv_nsec -= 1000000000;
    }

    // Wait until exact next time
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL);
  }

  */
   // encoders.init();

  

//parlcd_hx8357_init(parlcd_mem_base);
 /* parlcd_hx8357_init(parlcd_mem_base);

  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (i = 0; i < 320 ; i++) {
    for (j = 0; j < 480 ; j++) {
      c = 0;
      parlcd_write_data(parlcd_mem_base, c);
    }
  }

  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (i = 0; i < 320 ; i++) {
    for (j = 0; j < 480 ; j++) {
      c = ((i & 0x1f) << 11) | (j & 0xff);
      parlcd_write_data(parlcd_mem_base, c);
    }
  }*/
/*
  mem_base = (unsigned char*) map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

  if (mem_base == NULL)
    exit(1);

 struct timespec loop_delay = {.tv_sec = 0, .tv_nsec = 20 * 1000 * 1000};
  for (i=0; i<30; i++) {
     *(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = val_line;
     val_line<<=1;
     printf("LED val 0x%x\n", val_line);
     clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }
  
  
  unsigned char *parlcd_mem_base = (unsigned char*)  map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

  if (parlcd_mem_base == NULL)
    exit(1);

 // parlcd_hx8357_init(parlcd_mem_base);

  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (i = 0; i < 320 ; i++) {
    for (j = 0; j < 480 ; j++) {
      c = 0;
      parlcd_write_data(parlcd_mem_base, c);
    }
  }

  parlcd_write_cmd(parlcd_mem_base, 0x2c);
  for (i = 0; i < 320 ; i++) {
    for (j = 0; j < 480 ; j++) {
      c = ((i & 0x1f) << 11) | (j & 0x1f);
      parlcd_write_data(parlcd_mem_base, c);
    }
  }

  loop_delay.tv_sec = 0;
  loop_delay.tv_nsec = 200 * 1000 * 1000;
  for (k=0; k<60; k++) {
    
    parlcd_write_cmd(parlcd_mem_base, 0x2c);
    for (i = 0; i < 320 ; i++) {
      for (j = 0; j < 480 ; j++) {
        c = (((i+k) & 0x1f) << 11) | ((j+k) & 0x1f);
        parlcd_write_data(parlcd_mem_base, c);
      }
    }

     clock_nanosleep(CLOCK_MONOTONIC, 0, &loop_delay, NULL);
  }*/
 // }

  printf("Goodbye world\n");

  return 0;
}