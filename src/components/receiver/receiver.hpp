#include <SFML/Audio.hpp>
#include <algorithm>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <ctime>
#include <iostream>
#include <string>
using namespace sf;
using namespace std;

using boost::asio::ip::udp;

class Receiver {
public:
  Receiver();
  void start_receive();
  void handle_receive(const boost::system::error_code &error, std::size_t);
  // void handle_send(boost::shared_ptr<std::string> /*message*/,
  //                  const boost::system::error_code & /*error*/,
  //                  std::size_t /*bytes_transferred*/);
  void run();
  void PlayBuffer();

private:
  boost::asio::io_context io_context;
  udp::resolver resolver;
  udp::endpoint receiver_endpoint;
  udp::socket socket;
  boost::array<char, 1> send_buf;
  SoundBuffer recv_buf;
  udp::endpoint sender_endpoint;
  size_t len;

  unsigned int sampleRate = 44100;

  sf::SoundBuffer buffer;

  sf::Sound sound;
}