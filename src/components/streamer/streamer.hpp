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

class Streamer {
public:
  Streamer();
  void SelectDevice();
  void Record(unsigned int n = 0);
  void run();

private:
  unsigned int sampleRate = 44100;
  SoundBufferRecorder recorder;
  SoundBuffer buffer;
  Sound sound;

  boost::asio::io_context io_context;
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  boost::array<char, 1> recv_buffer_;

  void start_receive();

  void handle_receive(const boost::system::error_code &error,
                      std::size_t /*bytes_transferred*/);

  void handle_send(boost::shared_ptr<std::string> /*message*/,
                   const boost::system::error_code & /*error*/,
                   std::size_t /*bytes_transferred*/);
};