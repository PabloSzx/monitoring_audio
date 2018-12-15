#include "./components/audio/audio.hpp"
#include <bits/stdc++.h>

using namespace std;

int main() {
  auto a = new Audio();

  a->Record(3);

  a->PlayBuffer();

  free(a);
  return 0;
}