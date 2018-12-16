#include "../components/audio/audio.hpp"
#include "../components/network/client/udp.hpp"
#include <bits/stdc++.h>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main() {
  auto audio = new Audio();

  auto connection = new udp_client();

  return 0;
}