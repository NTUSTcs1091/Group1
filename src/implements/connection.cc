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
  boost::asio::async_read(
      socket_, boost::asio::buffer(buffer_.get(), buffer_size_),
      [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
        std::string message(buffer_.get());
        return message.find("\r\n") != std::string::npos;
      },
      strand_.wrap([this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
          auto rawBuffer = buffer_.get();
          std::string request_str(rawBuffer, bytes_transferred - 2);
          auto request = HttpUtils::HttpRequest(request_str);
          auto abs_pathname = boost::filesystem::current_path() / request.pathname;
          std::string pathStr(abs_pathname.c_str());
          auto response = HttpUtils::HttpResponse(pathStr);
          if (response.state_ == 200) {
            std::ifstream file(pathStr);
            std::string content((std::istreambuf_iterator<char>(file)),
                                (std::istreambuf_iterator<char>()));
            response.setMessage("OK").setContent(content);
          } else {
            response.setMessage("Not Found");
          }
          auto replyContent = response.stringify();
          memset(rawBuffer, 0, bytes_transferred);
          strncpy(rawBuffer, replyContent.c_str(), replyContent.size());
          write_(replyContent.size());
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
