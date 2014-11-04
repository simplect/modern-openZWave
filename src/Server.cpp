#include <json/json.h>
#include <iostream>
#include <zmq.hpp>

#include "Server.h"

namespace Modernozw {

    int startServer()
    {
        m_socket.bind("tcp://127.0.0.1:5678");
        return 0;
    }

    void serverThread()
    {
        return ;
        zmq::message_t reply(2);
        memcpy((void*)reply.data(), "ok", 2);
        while(true){
            zmq::message_t request;

            m_socket.recv(&request);
            std::cout << "Received " << request.data() << std::endl;
            m_socket.send(reply);
        }
    }

    void sendMessage(
            uint8_t nodeId,
            std::string type,
            std::string name,
            std::string value)
    {
            Json::Value message;
            message["nodeid"] = nodeId;
            message["type"] = type;
            message["name"] = name;
            message["value"] = value;
            Json::FastWriter writer;
            std::string jsonMessage = writer.write(message);
#if DEBUG
            std::cout << jsonMessage;
#endif
            zmq::message_t zmqMessage(jsonMessage.length());
            memcpy((void*)zmqMessage.data(), jsonMessage.c_str(), jsonMessage.length());
            m_socket.send(zmqMessage);
    }
}
