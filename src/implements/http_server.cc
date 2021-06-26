#include "include/http_server.hpp"

HttpServer::HttpServer(std::string fileRoot, ushort port)
    : rootpath_(fileRoot),
      acceptor_(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
  namespace fs = boost::filesystem;
  if (!fs::exists(rootpath_) && !fs::is_directory(rootpath_)) {
    throw std::runtime_error(rootpath_ + " isn't directory.\n");
  } else {
    fs::current_path(rootpath_);
    std::cout << fs::current_path() << "\n";
  }
}

void HttpServer::start() { accept_(); }

void HttpServer::accept_() {
  auto conn = std::make_shared<Connection>(io_context_);
  acceptor_.async_accept(conn->socket(), [this, conn](boost::system::error_code ec) {
    if (!ec) {
      std::cout << "Connection!\n";
      conn->start();
    }
    accept_();
  });

  io_context_.run();
}
