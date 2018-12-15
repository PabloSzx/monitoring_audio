#include "audio.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

void Audio::Record(unsigned int n) {
  std::cout << "Press enter to start recording audio";
  std::cin.ignore(10000, '\n');

  recorder.start(sampleRate);

  if (n > 0) {
    std::cout << "Recording for " << n << " seconds" << std::endl;
    for (int i = 0; i < n; i++) {
      std::cout << ".";
      std::flush(std::cout);
      sf::sleep(sf::seconds(1));
    }
    std::cout << std::endl;
  } else {
    std::cout << "Recording... press enter to stop";
    std::cin.ignore(10000, '\n');
    std::cout << std::endl;
  }
  recorder.stop();
  buffer = recorder.getBuffer();

  std::cout << "Sound information:" << std::endl;
  std::cout << " " << buffer.getDuration().asSeconds() << " seconds"
            << std::endl;
  std::cout << " " << buffer.getSampleRate() << " samples / seconds"
            << std::endl;
  std::cout << " " << buffer.getChannelCount() << " channels" << std::endl;
  sound.setBuffer(buffer);
}
void Audio::PlayBuffer() {

  sound.play();

  // Wait until finished
  while (sound.getStatus() == sf::Sound::Playing) {
    // Display the playing position
    std::cout << "\rPlaying... " << sound.getPlayingOffset().asSeconds()
              << " sec        ";
    std::cout << std::flush;

    // Leave some CPU time for other threads
    sf::sleep(sf::milliseconds(100));
  }

  std::cout << std::endl << "Done!" << std::endl;
}
Audio::Audio() {
  if (sf::SoundRecorder::isAvailable() == false) {
    std::cout << "Sorry, audio capture is not supported by your system"
              << std::endl;
    return;
  }
}
