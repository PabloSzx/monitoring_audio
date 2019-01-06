#include "receiver.hpp"
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

Receiver::Receiver() : resolver(io_context), socket(io_context) {

  receiver_endpoint =
      *resolver.resolve(udp::v4(), "localhost", "daytime").begin();

  socket.open(udp::v4());

  send_buf = {{0}};

  socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

  start_receive();
}

void Receiver::start_receive() {
  // socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

  socket.async_receive_from(
      SoundBuffer(recv_buf), sender_endpoint,
      boost::bind(&Receiver::handle_receive, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void Receiver::handle_receive(const boost::system::error_code &error,
                              std::size_t) {
  len = socket.receive_from(SoundBuffer(recv_buf), sender_endpoint);

  sound.setBuffer(buffer);

  PlayBuffer();
  // buffer = new SoundBuffer(recv_buf);
  //   std::cout.write(recv_buf.data(), len);

  //   start_receive();
}

void Receiver::run() {
  try {
    io_context.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}

void Receiver::PlayBuffer() {

  sound.play();

  // Wait until finished
  while (sound.getStatus() == sf::Sound::Playing) {
    // Display the playing position
    std::cout << "\rPlaying... " << sound.getPlayingOffset().asSeconds()
              << " sec        ";
    std::cout << std::flush;

    // Leave some CPU time for other threads
    sf::sleep(sf::milliseconds(100));
  }

  std::cout << std::endl << "Done!" << std::endl;
}
