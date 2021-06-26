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
#ifndef _GROUP1_HTTP_UTILS_H_
#define _GROUP1_HTTP_UTILS_H_
#include <map>
#include <iostream>
#include <sstream>
#include <boost/filesystem.hpp>

/**
 * @brief
 * Http Utils provider Encapsulation of Http request and response
 *
 */
namespace HttpUtils 
{


/**
 * @brief
 * HttpRequest receive request string, then try to parse
 * 
 * @example use HttpRequest
 * 
 * @code
 * auto request = HttpRequest(request_string);
 * for(auto header: headers) {
 *     do_sometring(header);
 * }
 * auto method = request.method; 
 * auto host = request.host; 
 * auto pathname = request.pathname;
 * auto rawData = request.rawData;
 */
struct HttpRequest {
    /**
     * Receive request_str, then parse
     */
    HttpRequest(std::string& request_str);

    /**
     * parse request headers, then encapsulate in map container.
     */
    std::map<std::string, std::string> headers;

    /**
     * Http Request method, ex :
     * if Browser open https://www.google.com/,
     * method equal "GET"
     */
    std::string method;

    /**
     * Http Request method, ex :
     * if Browser open https://www.google.com/,
     * pathname equal "/"
     */
    std::string pathname;

    /**
     * Http Request method, ex :
     * if Browser open https://www.google.com/,
     * rawData equal "GET / HTTP/1.1 ... <headers> ... <CRLF>"
     */
    std::string rawData;
};

/**
 * @brief
 * HttpResponse provider http response factor,
 * use setStatus, setMessage, setContent
 * 
 * @example use HttpRequest
 * 
 * @code
 * auto response = HttpResponse();
 * response.setStatus(200).setMessage("OK").setContent(content);
 * sendResponse(response.stringify());
 */
struct HttpResponse {

    /**
     * Accept a filename as a parameter,
     * file system will look for the existence of the file and create a response.
     */
    HttpResponse(const std::string& requestFile);

    /**
     * Set http response state code
     */
    struct HttpResponse& setStatus(ushort stateCode);

    /**
     * Set http response message
     */
    struct HttpResponse& setMessage(std::string message);

    /**
     * Set http response content
     */
    struct HttpResponse& setContent(std::string& content);

    /**
     * return http response format string
     */
    std::string stringify();

    /**
     * struct member, it save http state
     */
    ushort state_;

    /**
     * struct member, it save http message
     */
    std::string message_;

    /**
     * struct member, it save http content
     */
    std::string content_;
};

typedef struct HttpRequest HttpRequest;
typedef struct HttpResponse HttpResponse;

}

#endif
