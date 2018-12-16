#include "udp.hpp"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <ctime>
#include <iostream>
#include <string>

using boost::asio::ip::udp;

std::string make_daytime_string() {
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

udp_server::udp_server() : socket_(io_context, udp::endpoint(udp::v4(), 13)) {
  start_receive();
}

void udp_server::start_receive() {
  socket_.async_receive_from(
      boost::asio::buffer(recv_buffer_), remote_endpoint_,
      boost::bind(&udp_server::handle_receive, this,
                  boost::asio::placeholders::error,
                  boost::asio::placeholders::bytes_transferred));
}

void udp_server::handle_receive(const boost::system::error_code &error,
                                std::size_t /*bytes_transferred*/) {
  if (!error) {
    boost::shared_ptr<std::string> message(
        new std::string(make_daytime_string()));

    socket_.async_send_to(
        boost::asio::buffer(*message), remote_endpoint_,
        boost::bind(&udp_server::handle_send, this, message,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

    start_receive();
  }
}

void udp_server::handle_send(boost::shared_ptr<std::string> /*message*/,
                             const boost::system::error_code & /*error*/,
                             std::size_t /*bytes_transferred*/) {}

void udp_server::run() {
  try {
    io_context.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
}
