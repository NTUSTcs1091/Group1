#include "include/connection.hpp"

boost::asio::io_service Connection::service;

boost::asio::ip::tcp::socket& Connection::socket() { return socket_; };

void Connection::start() { read_(); };

long Connection::during(){
    auto now = std::chrono::system_clock::now();
    auto passTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_active_);
    return passTime.count();
};

void Connection::read_() {
  auto self = shared_from_this();
  socket_.async_read_some(boost::asio::buffer(buffer_),
                          [this, self](boost::system::error_code ec, size_t recv_bytes) {
                            if (!ec) {
                              write_(recv_bytes);
                            }
                          });
};

void Connection::write_(size_t length) {
  auto self = shared_from_this();
  boost::asio::async_write(socket_, boost::asio::buffer(buffer_, buffer_size_),
                           [this, self](boost::system::error_code ec, size_t recv_bytes) {
                             if (!ec) {
                               read_();
                             }
                           });
};
