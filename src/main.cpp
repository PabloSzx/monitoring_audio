#include "./components/audio/audio.hpp"
#include <bits/stdc++.h>

using namespace std;

int main() {
  auto a = new Audio();

  try {
    a->record(2);
    a->play(true);
    a->record(1, false);
    a->play();
    a->play();
    a->record(5);
    a->play();
  } catch (const std::exception &) {
    cout << "errorsito" << endl;
    Pa_Terminate();
  }

  // a->record(5);
  // a->play();
  // a->record(5);
  // a->play();

  return 0;
}