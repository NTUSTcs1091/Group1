#include "include/connection.hpp"

#include <iostream>

boost::asio::io_context Connection::service;

boost::asio::ip::tcp::socket& Connection::socket() { return socket_; };

void Connection::start() { read_(); };

long Connection::during() {
  auto now = std::chrono::system_clock::now();
  auto passTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_active_);
  return passTime.count();
};

void Connection::read_() {
  auto self = shared_from_this();
  socket_.async_read_some(
      boost::asio::buffer(buffer_.get(), buffer_size_),
      strand_.wrap([this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
          write_(bytes_transferred);
        }
      }));
}

void Connection::write_(std::size_t length) {
  auto self = shared_from_this();
  boost::asio::async_write(
      socket_, boost::asio::buffer(buffer_.get(), length),
      strand_.wrap([this, self](boost::system::error_code ec, std::size_t /* bytes_transferred */) {
        if (!ec) {
          read_();
        }
      }));
}
