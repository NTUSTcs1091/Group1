#include <cstring>
#include <iostream>

#include "include/http_server.hpp"

int main(int argc, char const *argv[]) {
  ushort port = -1;
  std::string root;
  for (int i = 1; i < argc; ++i) {
    if (strcmp(argv[i], "-d") == 0) {
      root = std::string(argv[i + 1]);
    } else if (strcmp(argv[i], "-p") == 0) {
      port = atoi(argv[i + 1]);
    }
  }

  std::cout << "Server running at port:" << port << " , serve ";
  auto server = HttpServer(root, port);
  server.start();

  return 0;
}
