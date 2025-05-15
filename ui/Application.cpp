#include "Application.h"

Application::Application() : list(WIDTH, HEIGHT) {

}

void Application::start() {
    vuMeter.init();
    vuMeter.setMaxVal(0xFFFF);
    lcd.init();
    dac.init();

    lcd.setView(&list);
    openDir("/");
}

bool Application::isPlayingTrack() {
    return mediaPlayer != NULL && mediaPlayer->isPlaying();
}

bool Application::hasPausedTrack() {
  return mediaPlayer != NULL && !mediaPlayer->isPlaying();
}

ListView* Application::getMainListView() {
    return &this->list;
}

MediaPlayer* Application::getMediaPlayer() {
    return this->mediaPlayer;;
}

void Application::showPlayingView() {
    lcd.setView(&playingView);
    playingView.setMediaPlayer(mediaPlayer);
    playingView.update();
}

LCD* Application::getLCD() {
    return &this->lcd;
}

void Application::setVolume(double volume) {

  if (mediaPlayer != NULL) {
    mediaPlayer->setVolume(volume);
  }

  View* previousView = lcd.getCurrentView();


  lcd.setView(&volumeView);
  volumeView.setValue(volume);
  lcd.setView(previousView);
  this->volume = volume;  

  Executor executor;
  executor.runAfter([this, previousView]() {

    printf("test\n");
    previousView->update();

  }, 2000000, true);
}

double Application::getVolume() {
  return this->volume;
}

void Application::openDir(std::string name) {
  fileManager.openDir(name);

  list.clear();
  for (File file : fileManager.listFiles())  {
    list.addItem(file.getName(), DIRECTORY);
  }

  list.update();
}

bool Application::showingPlayingView() {
  return lcd.getCurrentView() == &playingView;
}

void Application::openSelectedFile() {
    int selected = list.getSelectedIndex();
    std::vector<File>& files = fileManager.listFiles();

    openFile(files, selected);
}

void Application::openFile(std::vector<File>& files, int index) {
  File file = files[index];
  switch (file.getType()) {
    case CHILD_DIR:
    case PARENT_DIR:
      openDir(file.getName());
      break;
    
    case SOUND_FILE:
      if (mediaPlayer != NULL && mediaPlayer->isPlaying()) {
          mediaPlayer->stop();
          free(mediaPlayer);
      }

      std::vector<std::string> playlist;
      int startTrack = 0;
      for (int i = 0; i < files.size(); i++) {
        if (files[i].getType() == SOUND_FILE) {
          if (i == index) startTrack = playlist.size();
          playlist.push_back(files[i].getAbsoulutePath());
        }
      }

      mediaPlayer = new MediaPlayer(playlist);
      mediaPlayer->setOutput(&dac);
      mediaPlayer->showAudioLevel(&vuMeter);
      mediaPlayer->setVolume(volume);
      
      mediaPlayer->setOnPositionUpdate([this](int position) {
          if (lcd.getCurrentView() == &playingView) playingView.updateSeekBar(position);
      });

      mediaPlayer->play(startTrack);
      break;
  }
}