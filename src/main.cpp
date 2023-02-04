#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <utility>

#include "tcpserver.h"

int
main(int argc, char *argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Usage: async_tcp_echo_server <port>\n";
            return 1;
        }

        TcpServer server(std::atoi(argv[1]));

        server.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
