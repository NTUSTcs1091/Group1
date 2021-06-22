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
#ifndef _GROUP1_CONNECTION_H_
#define _GROUP1_CONNECTION_H_
#include <boost/asio.hpp>
#include <chrono>
#include <memory>
#include <thread>
#include <vector>
/**
 * @brief
 * This class corresponds to the session layer of the network OSI model,
 * when the operating system establishes a new connection,
 * Connection constructor will create a socket, and bind socket to I/O object.
 * I/O object provided by asio::io_service,
 * it's equivalent to creating a layer of I/O Context in the operating system and application.
 * 
 * @example Connection application scenarios
 * 
 * @code 
 * asio::acceptor acc;
 * auto connection = make_shared<Connection>();
 * acc.accept(connection);
 * connection.start();
 */
class Connection : std::enable_shared_from_this<Connection> {
 public:
   Connection(Connection&) = delete;
   Connection& operator=(Connection&) = delete;
  /**
   * Every socket should bind to io_service,
   * according to boost::asio document, io_service.run() is thread safe,
   * Therefore declared as static member.
   * when constructor is called, pass to the newly created socket.
   */
  static boost::asio::io_service service;

  /**
   * create a socket and initialize a buffer.
   * According to asio official documents, a 4KB buffer should be able to handle most message.
   */
  Connection(uint buffer_size = 4096): socket_(Connection::service), buffer_size_(buffer_size) {
      buffer_.reserve(buffer_size_);
      last_active_ = std::chrono::system_clock::now();
  };

  /**
   * return socket instance, usually acceptor will use it.
   */
  boost::asio::ip::tcp::socket& socket();

  /**
   * it just call read_() function.
   * Please note: it should be executed after Connection::service.run() called.
   * 
   * In asynchronous model, Ex. use async_read(), async_write(),
   * callback function will enqueue to I/O Executor (Please refer to official documents)
   * If io_service.run() is not executed, asio WON'T CHECK I/O Executor queue,
   * This will result in no response from the appliaction.
   */
  void start();

  /**
   * read_(), write_() will update last_active_,
   * this function will return time difference 
   * between last_active_ current of the function call,
   * the unit is microsecond.
   */
  long during();
 private:
  /**
   * read_ will read the data packet from the I/O object,
   * then enqueue callback to I/O Executor queue.
   * This is very important, please call read_() or write_() again at the end of callback.
   * The reason is as mentioned before, if not enqueue executor to I/O Executor queue,
   * will cause the I/O Executor queue to be empty.
   */
  void read_();

  /**
   * The behavior of this function is similar to read_, 
   * except that it will pass the data to the I/O object, 
   * and then send it out by the I/O object.
   * Same as above, should call read_() or write_() again after callback.
   */
  void write_(size_t length);

  /**
   * socket instance, providing read/write interface
   */
  boost::asio::ip::tcp::socket socket_;

  /**
   * Data storage buffer. The original design is to split the read/write buffer,
   * But later found out that every time the request must be read first, and then responded
   * Can only read or write at the same time, 
   * so there is no need to specially disassemble the buffer for reading and writing.
   */
  std::vector<char> buffer_;

  /**
   * read_(), write_() will update this value,
   * the value is equivalent to the system time point of the last read and write operation
   */
  std::chrono::time_point<std::chrono::high_resolution_clock> last_active_;


  /**
   * allocate memory size to buffer, unit is bits
   */
  size_t buffer_size_;
};
#endif //_GROUP1_CONNECTION_H_
