#include <SFML/Audio.hpp>
//#include <bits/stdc++.h>
#include <iostream>

class Audio {
public:
  Audio();

private:
  unsigned int sampleRate = 44100;
  sf::SoundBufferRecorder recorder;
  sf::SoundBuffer buffer;
  sf::Sound sound;
};