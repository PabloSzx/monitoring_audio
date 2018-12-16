#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::udp;

class udp_client {
public:
  udp_client();

private:
  boost::asio::io_context io_context;
  udp::resolver resolver;
  udp::endpoint receiver_endpoint;
  udp::socket socket;
  boost::array<char, 1> send_buf;
  boost::array<char, 128> recv_buf;
  udp::endpoint sender_endpoint;
  size_t len;
};