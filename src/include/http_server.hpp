/*
 * MIT License
 * Copyright (c) 2021 Yen Hao, Chen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _GROUP1_HTTP_SERVER_H_
#define _GROUP1_HTTP_SERVER_H_
#include <boost/asio.hpp>
#include <exception>
#include <memory>

#include "include/connection.hpp"
#include "include/thread_pool.hpp"

/**
 * @brief
 *
 * @example
 *
 * @code
 *
 */
class HttpServer {
 public:
  HttpServer(std::string fileRoot, ushort port);

  void start();

 private:
  void accept_();

  std::string rootpath_;
  boost::filesystem::path workdir_;
  boost::asio::io_service &io_context_ = Connection::service;
  boost::asio::ip::tcp::acceptor acceptor_;
};

#endif  // _GROUP1_HTTP_SERVER_H_
