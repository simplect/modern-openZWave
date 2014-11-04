#ifndef _SERVER_HEADER
#define _SERVER_HEADER

#include <zmq.hpp>

namespace Modernozw {
    int startServer();
    void sendMessage(
            uint8_t nodeid,
            std::string type,
            std::string name,
            std::string value);

        zmq::context_t context (1);
        zmq::socket_t m_socket (context, ZMQ_PUB);

}
#endif
