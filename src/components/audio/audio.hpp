#include <SFML/Audio.hpp>
//#include <bits/stdc++.h>
#include <iostream>

class Audio {
public:
  Audio();
  void Record(unsigned int n = 0);
  void PlayBuffer();
  void SelectDevice();

private:
  unsigned int sampleRate = 44100;
  sf::SoundBufferRecorder recorder;
  sf::SoundBuffer buffer;
  sf::Sound sound;
};