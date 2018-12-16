#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::udp;

class udp_client {
public:
  udp_client();
  void start_receive();
  void handle_receive(const boost::system::error_code &error, std::size_t);
  // void handle_send(boost::shared_ptr<std::string> /*message*/,
  //                  const boost::system::error_code & /*error*/,
  //                  std::size_t /*bytes_transferred*/);
  void run();

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