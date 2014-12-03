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
        m_ssocket.bind("tcp://*:5555");
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
            try {
                int homeId = (int)jsonRequest["home_id"].asInt();
                if(homeId != (int)g_homeId && homeId != 0 && homeId){
                    continue;
                }
                Node * node;
                if(!(node = getNode(jsonRequest["node_id"].asInt()))){
                    continue;
                }
                node->setValue(jsonRequest["value"].asBool());
            } catch(std::runtime_error error){

            }
        }
    }

    void sendJson(Json::Value message)
    {
        Json::FastWriter writer;
        std::string jsonMessage = writer.write(message);
#if DEBUG
        std::cout << jsonMessage;
#endif
        zmq::message_t zmqMessage(jsonMessage.length());
        memcpy((void*)zmqMessage.data(), jsonMessage.c_str(), jsonMessage.length());
        m_csocket.send(zmqMessage);
    }

    void sendString(
            uint8_t nodeId,
            uint32_t homeId,
            std::string type,
            std::string name,
            std::string value)
    {
        using namespace OpenZWave;
        Json::Value message;
        message["node_id"] = nodeId;
        message["home_id"] = homeId;
        message["type"] = type;
        message["name"] = name;
        message["value"] = value;
        message["symbol"] = "zwave";

        sendJson(message);
    }

    void sendInt(
            uint8_t nodeId,
            uint32_t homeId,
            std::string type,
            std::string name,
            uint8_t value)
    {
        using namespace OpenZWave;
        Json::Value message;
        message["node_id"] = nodeId;
        message["home_id"] = homeId;
        message["type"] = type;
        message["name"] = name;
        message["valuetype"] = "int";
        message["value"] = value;
        message["symbol"] = "zwave";

        sendJson(message);
    }



    void sendValue(
            uint8_t nodeId,
            uint32_t homeId,
            std::string type,
            OpenZWave::ValueID value)
    {
        using namespace OpenZWave;
        Json::Value message;
        message["node_id"] = nodeId;
        message["home_id"] = homeId;
        message["type"] = type;
        message["name"] = Manager::Get()->GetValueLabel(value);
        message["symbol"] = "zwave";

        switch(value.GetType()){
            case ValueID::ValueType::ValueType_Int:
                message["valuetype"] = "int";
                int tempint;
                Manager::Get()->GetValueAsInt(value, &tempint);
                message["value"] = tempint;
                break;
            case ValueID::ValueType::ValueType_Bool:
                message["valuetype"] = "bool";
                bool tempbool;
                Manager::Get()->GetValueAsBool(value, &tempbool);
                message["value"] = tempbool;
                break;
            case ValueID::ValueType::ValueType_Decimal:
                message["valuetype"] = "float";
                float tempfloat;
                Manager::Get()->GetValueAsFloat(value, &tempfloat);
                message["value"] = tempfloat;
                break;
            case ValueID::ValueType::ValueType_String:
            default:
                message["valuetype"] = "string";
                std::string tempstr;
                Manager::Get()->GetValueAsString(value, &tempstr);
                message["value"] = tempstr;

        }

        sendJson(message);
    }


}
