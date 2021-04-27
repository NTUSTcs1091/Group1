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
 */
#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include <memory>
#include <utility>
#include <string>

#include <boost/asio.hpp>

#include "SocketContext.hpp"
/**
 * 
 * This file provides a bridge between the Tcp Server and the system socket,
 * Through the pre-defined SocketContext, linked to the acceptor, 
 * run asio::io_context.run(), it will combine the system buffer, socket, and SocketContext. 
 *
 * Ex.
 * int main()
 * {
 *     ip::tcp::socket socket;
 *     SocketContext(socket) sc;
 *     sc.setBufferSize(1024);
 *     sc.createReadEventTrigger([](std::string data){
 *         httpParser(data);
 *         DoSomethingWhenRead(data);
 *     });
 *     sc.createWriteEventTrigger([](std::string data){
 *         loggerThat(data);
 *         DoSomethingWhenWrite(data);
 *     });
 *     boost::asio::io_context io_context;
 *     TcpServer server(std::make_shared<SocketContext>(tc), io_context, 8088);
 *     server.run();
 * }
 */
template <typename T, typename U>
class TcpServer
{
public : 
    TcpServer(const TcpServer &) = delete;
    TcpServer &operator=(const TcpServer &) = delete;
    TcpServer(
        const std::shared_ptr<SocketContext> scontext_sptr, 
        boost::asio::io_context &io_context, 
        unsigned short &port
    );

    //Start listening on the specified port
    void run();
private:
    // The callback when a new connection comes, except for events defined by the developer
    // will definitely trigger SocketContext.start()
    void accept();
    boost::asio::ip::tcp::acceptor acceptor_;
    std::shared_ptr<SocketContext> socket_;
};
#endif //TCP_SERVER_H_
