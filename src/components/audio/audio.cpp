#include "audio.hpp"
#include <SFML/Audio.hpp>
#include <iostream>

void Audio::Record(unsigned int n) {
  recorder.start(sampleRate);
  buffer = recorder.getBuffer();
  std::cout << "Recording... press enter to stop";
  std::cin.ignore(10000, '\n');
  std::cout << std::endl;
  recorder.stop();

  std::cout << "Sound information:" << std::endl;
  std::cout << " " << buffer.getDuration().asSeconds() << " seconds"
            << std::endl;
  std::cout << " " << buffer.getSampleRate() << " samples / seconds"
            << std::endl;
  std::cout << " " << buffer.getChannelCount() << " channels" << std::endl;

  sound.setBuffer(buffer);

  sound.play();

  // Wait until finished
  while (sound.getStatus() == sf::Sound::Playing) {
    // Display the playing position
    std::cout << "\rPlaying... " << sound.getPlayingOffset().asSeconds()
              << " sec        ";
    std::cout << std::flush;

    // Leave some CPU time for other threads
    sf::sleep(sf::milliseconds(100));

    std::cout << std::endl << "Done!" << std::endl;

    // Wait until the user presses 'enter' key
    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(10000, '\n');

    return;
  }
}
void Audio::PlayBuffer() {}
Audio::Audio() {
  if (sf::SoundRecorder::isAvailable() == false) {
    std::cout << "Sorry, audio capture is not supported by your system"
              << std::endl;
    return;
  }
}
