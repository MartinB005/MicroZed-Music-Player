#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>

#include "core/mzapo_parlcd.h"
#include "core/mzapo_phys.h"
#include "core/mzapo_regs.h"
#include "ui/font_types.h"
#include "peripherals/LCD.h"
#include "peripherals/DAC.h"
#include "audio/MediaPlayer.h"
#include "ui/ListView.h"
#include "ui/View.h"
#include "utils/FileBrowser.h"
#include "peripherals/Encoders.h"
#include "utils/Executor.h"
#include "ui/icons.h"


DAC dac;
VUMeter vuMeter;


void openDir(FileBrowser* browser, std::string name, ListView* list) {
  browser->openDir(name);
  std::vector<File> files = browser->listFiles();

  for (File file : files)  {
    list->addItem(file.getName().c_str(), DIRECTORY);
  }
}

void openFile(FileBrowser* browser, MediaPlayer* mediaPlayer, File file, ListView* list) {
  switch (file.getType()) {
  case CHILD_DIR:
  case PARENT_DIR:
    openDir(browser, file.getName(), list);
    break;
  
  case SOUND_FILE:
    mediaPlayer = new MediaPlayer("/home/Music/style.wav");
    mediaPlayer->setOutput(&dac);
    mediaPlayer->showAudioLevel(&vuMeter);
    mediaPlayer->play();
  
  default:
    break;
  }
}

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


  LCD lcd;
  ListView list(WIDTH, HEIGHT);
  FileBrowser browser;
  Encoders encoders;

  vuMeter.init();
  vuMeter.setMaxVal(0xFFFF);
  encoders.init();
  lcd.init();
  dac.init();

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
  
  

  MediaPlayer* mediaPlayer = NULL;



  lcd.setView(&list);
  //lcd.whiteScreen();
  printf("%p", list.getPixels());
//list.addItem("test", DIRECTORY);
  
/*
  browser.openDir("/");
  std::vector<File> files = browser.listFiles();

  for (File file : files)  {
    list.addItem(file.getName().c_str(), DIRECTORY);
  }*/
  
  


 /*mem_base = (unsigned char*) map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
  
  uint32_t val = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
  printf("val %d\n", val);*/
  /* Store the read value to the register controlling individual LEDs */
  //*(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = val;
  
 


  encoders.setOnValueChange(RED, [&mediaPlayer] (bool clockwise) {
    if (mediaPlayer != NULL) {
      double change = clockwise ? 0.02 : -0.02;
      double newVol = mediaPlayer->getVolume() + change;
      if (newVol < 0) newVol = 0;
      else if (newVol > 1) newVol = 1;
      
      mediaPlayer->setVolume(newVol);
      printf("volume %.2f\n", newVol);
    }
  });

  encoders.setOnValueChange(GREEN, [&list] (bool clockwise) {
     clockwise ? list.selectNext() : list.selectPrevious();
  });
  

  encoders.setOnValueChange(BLUE, [] (bool clockwise) {
  //  printf("blue %d\n", clockwise);
  });
  

  encoders.setOnPress(RED, [] () {
    printf("red\n");
  });

 encoders.setOnPress(GREEN, [&list, &browser, &mediaPlayer] () {
    /*int selected = list.getSelectedIndex();
    File file = files[selected];
    openFile(&browser, mediaPlayer, file, &list);*/
  });

encoders.setOnPress(BLUE, [&mediaPlayer] () {
    printf("blue\n");
    mediaPlayer = new MediaPlayer("/home/Music/style.wav");
    mediaPlayer->setOutput(&dac);
    mediaPlayer->showAudioLevel(&vuMeter);
    mediaPlayer->play();
  });


  while (true) {
    if (mediaPlayer->isPlaying()) usleep(5000); 
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