#include "./components/audio/audio.hpp"
#include <bits/stdc++.h>

using namespace std;

int main() {
  auto a = new Audio();

  a->record(1);
  a->play();
  // a->record(1);
  // a->play();
  // a->record(1);
  // a->play();
  return 0;
}