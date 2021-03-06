#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <ctime>
#include <iostream>
#include <string>

using boost::asio::ip::udp;

class udp_server {
public:
  udp_server();
  void run();

private:
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
