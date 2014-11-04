#ifndef _SERVER_HEADER
#define _SERVER_HEADER

#include <zmq.hpp>

namespace Modernozw {
    int startServer();
    void sendMessage(
            uint8_t nodeId,
            uint32_t homeId,
            std::string type,
            std::string name,
            std::string value);

        zmq::context_t context (1);
        zmq::socket_t m_ssocket (context, ZMQ_SUB);
        zmq::socket_t m_csocket (context, ZMQ_PUB);

}
#endif
