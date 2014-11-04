#include <json/json.h>
#include <iostream>
#include <zmq.hpp>

#include "Server.h"
#include "Main.h"
#include "MNode.h"

namespace Modernozw {

    int startServer()
    {
        m_ssocket.bind("tcp://127.0.0.1:5555");
        zmq_setsockopt(m_ssocket, ZMQ_SUBSCRIBE, "", 0);
        m_csocket.bind("tcp://127.0.0.1:5556");
        return 0;
    }

    void serverThread()
    {
        while(true){
            zmq::message_t request;
            m_ssocket.recv(&request);
            char* strRequest = (char*)request.data();
#if DEBUG
            std::cout << "Received " << strRequest << std::endl;
#endif
            Json::Value jsonRequest;
            Json::Reader reader;
            //parse the request, if invalid drop it
            if(!reader.parse(strRequest, jsonRequest)){
                continue;
            }

            if(jsonRequest["homeid"].asInt() != (int)g_homeId){
                continue;
            }
            Node * node;
            if(!(node = getNode(jsonRequest["nodeid"].asInt()))){
                continue;
            }
            node->setValue(jsonRequest["value"].asBool());
        }
    }

    void sendMessage(
            uint8_t nodeId,
            uint32_t homeId,
            std::string type,
            std::string name,
            std::string value)
    {
            Json::Value message;
            message["nodeid"] = nodeId;
            message["homeid"] = homeId;
            message["type"] = type;
            message["name"] = name;
            message["value"] = value;
            message["symbol"] = "zwave";
            Json::FastWriter writer;
            std::string jsonMessage = writer.write(message);
#if DEBUG
            std::cout << jsonMessage;
#endif
            zmq::message_t zmqMessage(jsonMessage.length());
            memcpy((void*)zmqMessage.data(), jsonMessage.c_str(), jsonMessage.length());
            m_csocket.send(zmqMessage);
    }
}