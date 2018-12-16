#include "udp.hpp"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

using boost::asio::ip::udp;
using namespace std;

udp_client::udp_client() : resolver(io_context), socket(io_context) {

  receiver_endpoint =
      *resolver.resolve(udp::v4(), "localhost", "daytime").begin();

  socket.open(udp::v4());

  send_buf = {{0}};

  socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

  start_receive();
}

void udp_client::start_receive() {
  // socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

  socket.async_receive_from(
      boost::asio::buffer(recv_buf), sender_endpoint,
      boost::bind(&udp_client::handle_receive, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void udp_client::handle_receive(const boost::system::error_code &error,
                                std::size_t) {
  len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

  std::cout.write(recv_buf.data(), len);

  start_receive();
}

void udp_client::run() {
  try {
    io_context.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
