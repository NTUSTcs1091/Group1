/**   
 * MIT License
 *
 * Copyright (c) 2021 Yen Hao,Chen
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
 * 
 * This file provides the socket read/write context,
 * and will encapsulate asio::io_context,
 * encapsulating the io_context and socket read/write.
 *    
 * start() will open the socket's listening mode,
 * read boost::asio::buffer into data_, when the data has been processed,
 * use async_write to write to the socket,
 * and write back the data you want to send back to the socket.
 *
 * createReadEventTrigger(), createWriteEventTrigger()
 * they can be aggregated before and after reading and writing data.
 * 
 * SocketContext will encapsulate the events related to the socket and pass the entity to TcpServer for use.
 * 
 * Ex.
 * int main()
 * {
 *     ip::tcp::socket socket;
 *     TcpSession(socket) tc;
 *     tc.setBufferSize(1024);
 *     tc.createReadEventTrigger([](std::string data){
 *         httpParser(data);
 *         DoSomethingWhenRead(data);
 *     });
 *     tc.createWriteEventTrigger([](std::string data){
 *         loggerThat(data);
 *         DoSomethingWhenWrite(data);
 *     });
 *  
 *     TcpServer server(tc,)
 * }
 */
#ifndef SOCKET_CONTEXT_H_
#define SOCKET_CONTEXT_H_

#include <memory>
#include <utility>
#include <string>
#include <functional>
#include <boost/asio.hpp>

namespace asio = boost::asio;
namespace ip = boost::asio::ip;
using ErrorCode = boost::system::error_code;

template<typename T, typename U>
class SocketContext : public std::enable_shared_from_this<SocketContext>
{
public:
    SocketContext(const SocketContext &) = delete;
    SocketContext &operator=(const SocketContext &) = delete;
    SocketContext(ip::tcp::socket socket);
    void start();
    void setBufferSize(unsigned int& size);
    void createReadEventTrigger(std::function<T> &cb);
    void createWriteEventTrigger(std::function<U> &cb);

private:
    std::function<T> readEvent_;
    std::function<U> writeEvent_;
    ip::tcp::socket socket_;
    std::string data_;
    std::size_t bufferSize_;
};
#endif