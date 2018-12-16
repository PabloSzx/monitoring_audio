#include "udp.hpp"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::udp;
using namespace std;

udp_client::udp_client() : resolver(io_context), socket(io_context) {

  receiver_endpoint =
      *resolver.resolve(udp::v4(), "localhost", "daytime").begin();
  socket.open(udp::v4());
  send_buf = {{0}};
  socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);
  len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
  std::cout.write(recv_buf.data(), len);
}