#include "../components/audio/audio.hpp"
//#include "../components/network/server/udp.hpp"
#include "../components/streamer/streamer.hpp"
#include <bits/stdc++.h>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

int main() {
  auto audio = new Audio();
  audio->SelectDevice();
  audio->Record();

  audio->PlayBuffer();
  // auto streamer = new Streamer();
  // streamer->Start();
  // auto server = new udp_server();
  // server->run();
  return 0;
}