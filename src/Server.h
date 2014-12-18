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

#ifndef _SERVER_HEADER
#define _SERVER_HEADER

#include <zmq.hpp>
#include <json/json.h>

namespace Modernozw {
    int startServer();

    void sendJson(Json::Value message);
    void sendString(
            uint8_t nodeId,
            uint32_t homeId,
            std::string type,
            std::string name,
            std::string value);
    void sendValue(
            uint8_t nodeId,
            uint32_t homeId,
            std::string type,
            OpenZWave::ValueID value);
    void sendInt(
            uint8_t nodeId,
            uint32_t homeId,
            std::string type,
            std::string name,
            uint8_t value);

        zmq::context_t context(1);
        zmq::socket_t m_ssocket(context, ZMQ_REP);
        zmq::socket_t m_csocket(context, ZMQ_PUB);
        zmq::message_t  * m_okayMessage;

}
#endif
