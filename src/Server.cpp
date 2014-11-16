// --------------------------------------------------------------------------
// Server.cpp
// Modern-openZWave is a modern and optimized implementation
// of the openZWave library
// Copyright (C) 2014  Merijn Testroote

// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// --------------------------------------------------------------------------

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

            if(jsonRequest["home_id"].asInt() != (int)g_homeId){
                continue;
            }
            Node * node;
            if(!(node = getNode(jsonRequest["node_id"].asInt()))){
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
            message["node_id"] = nodeId;
            message["home_id"] = homeId;
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
