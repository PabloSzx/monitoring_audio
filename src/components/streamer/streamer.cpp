#include "streamer.hpp"
#include <SFML/Audio.hpp>
#include <algorithm>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;
using boost::asio::ip::udp;

Streamer::Streamer() : socket_(io_context, udp::endpoint(udp::v4(), 13)) {
  if (sf::SoundRecorder::isAvailable() == false) {
    std::cout << "Sorry, audio capture is not supported by your system"
              << std::endl;
    return;
  }

  SelectDevice();

  Record();

  start_receive();
}

void Streamer::Record(unsigned int n) {
  std::cout << "Press enter to start recording audio";
  std::cin.ignore(10000, '\n');

  recorder.start(sampleRate);

  if (n > 0) {
    std::cout << "Recording for " << n << " seconds" << std::endl;
    for (int i = 0; i < n; i++) {
      std::cout << ".";
      std::flush(std::cout);
      sf::sleep(sf::seconds(1));
    }
    std::cout << std::endl;
  } else {
    std::cout << "Recording... press enter to stop";
    std::cin.ignore(10000, '\n');
    std::cout << std::endl;
  }
  recorder.stop();
  buffer = recorder.getBuffer();

  std::cout << "Sound information:" << std::endl;
  std::cout << " " << buffer.getDuration().asSeconds() << " seconds"
            << std::endl;
  std::cout << " " << buffer.getSampleRate() << " samples / seconds"
            << std::endl;
  std::cout << " " << buffer.getChannelCount() << " channels" << std::endl;
  sound.setBuffer(buffer);
}

void Streamer::SelectDevice() {
  auto devices = recorder.getAvailableDevices();

  cout << "Choose a device:" << endl;

  auto cont = 0;
  int n;
  for (auto i : devices) {
    cout << cont << " : " << i << endl;
    cont++;
  }

  cout << "Put your chosen number: ";
  cin >> n;
  cout << endl;
  cin.ignore();

  recorder.setDevice(devices[n]);
}

void Streamer::start_receive() {
  socket_.async_receive_from(
      boost::asio::buffer(recv_buffer_), remote_endpoint_,
      boost::bind(&Streamer::handle_receive, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void Streamer::handle_receive(const boost::system::error_code &error,
                              std::size_t /*bytes_transferred*/) {
  if (!error) {
    boost::shared_ptr<SoundBuffer> message(new SoundBuffer(buffer));

    socket_.async_send_to(
        message, remote_endpoint_,
        boost::bind(&Streamer::handle_send, this, message,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

    start_receive();
  }
}

void Streamer::handle_send(boost::shared_ptr<std::string> /*message*/,
                           const boost::system::error_code & /*error*/,
                           std::size_t /*bytes_transferred*/) {
  boost::shared_ptr<SoundBuffer> message(new SoundBuffer(buffer));

  socket_.async_send_to(
      message, remote_endpoint_,
      boost::bind(&Streamer::handle_send, this, message,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));

  cout << "Enviado!" << endl;
}

void Streamer::run() {
  try {
    io_context.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
