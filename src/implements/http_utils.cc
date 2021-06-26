#include "include/http_utils.hpp"

HttpUtils::HttpRequest::HttpRequest(std::string& request_str) {
  std::stringstream ss(request_str);
  std::string result_temp;

  /* Iterate Http request string */
  bool pathResolved = false, setMethod = false, setPath = false;
  while (std::getline(ss, result_temp, '\n')) {
    /* First, process path information */
    if (!pathResolved) {
      std::string::size_type strBegin = 0;
      std::string::size_type strEnd = 0;
      while ((strEnd = result_temp.find(' ', strBegin)) != std::string::npos) {
        auto temp = result_temp.substr(strBegin, strEnd - strBegin);
        if (!setMethod) {
          method = temp;
          setMethod = true;
        } else if (!setPath) {
          pathname = temp;
          setPath = true;
        }

        strBegin = strEnd + 1;
        strEnd = result_temp.find(' ', strEnd);
      }

      /* remove query string */
      auto query = pathname.find('?');
      if (query != std::string::npos) {
        pathname = pathname.substr(0, query);
      }
      pathResolved = true;
      continue;
    }

    /*Second, deal with the header*/
    auto delim = result_temp.find(": ");
    auto headerKey = result_temp.substr(0, delim);
    auto content = result_temp.substr(delim + 2);
    headers[headerKey] = content;
  }
  rawData = request_str;
}

HttpUtils::HttpResponse::HttpResponse(const std::string& requestFile) {
  namespace fs = boost::filesystem;
  if (fs::exists(requestFile)) {
    std::cout << requestFile << " exists.\n";
    state_ = 200;
  } else {
    std::cout << requestFile << " NOT exists.\n";
    state_ = 404;
  }
}

HttpUtils::HttpResponse& HttpUtils::HttpResponse::setStatus(ushort stateCode) {
  state_ = stateCode;
  return *this;
}

HttpUtils::HttpResponse& HttpUtils::HttpResponse::setMessage(std::string message) {
  message_ = message;
  return *this;
}

HttpUtils::HttpResponse& HttpUtils::HttpResponse::setContent(std::string& content) {
  content_ = content;
  return *this;
}

std::string HttpUtils::HttpResponse::stringify() {
  std::stringstream resContent;
  resContent << "HTTP/1.1 " << state_ << " " << message_ << "\r\n";
  if(state_ == 200) {
    resContent << "content-type: text/plain\r\n" 
               << "content-length: " << content_.size() << "\r\n"
               << "\r\n" << content_;
  } else {
    resContent << "content-type: text/plain\r\n";
  }

  return resContent.str();
};
