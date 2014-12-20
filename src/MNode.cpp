// --------------------------------------------------------------------------
// MNode.cpp
// modern-openZWave is a modern and optimized implementation
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

#include <stdint.h>
#include <condition_variable>
#include <sstream>
#include "ValueStore.h"
#include "Notification.h"
#include "Manager.h"

#include "MNode.h"
#include "Main.h"
#include "Server.h"

namespace Modernozw {

    Node::Node(uint8_t nodeId, uint32_t homeId)
    {
        m_nodeId = nodeId;
        m_homeId = homeId;
    }

    Node::Node(uint8_t nodeId)
    {
        m_nodeId = nodeId;
        m_homeId = g_homeId;
    }

    void Node::nodeEvent(const OpenZWave::Notification *notification)
    {
        sendInt(m_nodeId,
                m_homeId,
                "event",
                OpenZWave::Manager::Get()->GetValueLabel(notification->GetValueID()),
                (uint8_t)notification->GetEvent());
    }

    void Node::valueChanged(const OpenZWave::Notification *notification)
    {
        using namespace OpenZWave;
        ValueID value = notification->GetValueID();
        /*Remove the old value
        for(auto it = m_values.begin(); it != m_values.end(); ++it){
            if((*it) == notification->GetValueID()){
                m_values.erase(it);
                break;
            }
        }*/
        //m_values.push_back(value);

        sendValue(
                m_nodeId,
                m_homeId,
                "valuechanged",
                value);
    }

    void Node::setValue(int32_t value, uint8_t commandClassId)
    {
        using namespace OpenZWave;
        for (auto it = m_values.begin(); it != m_values.end(); ++it) {
            ValueID valueId = *it;
            if (valueId.GetCommandClassId() == commandClassId) {
                Manager::Get()->SetValue(valueId, value);
                break;
            }
        }
    }

    void Node::setValue(bool value, uint8_t commandClassId)
    {
        using namespace OpenZWave;
        for (auto it = m_values.begin(); it != m_values.end(); ++it) {
            ValueID valueId = *it;
            if (valueId.GetCommandClassId() == commandClassId) {
                Manager::Get()->SetValue(valueId, value);
                break;
            }
        }

    }

    Node* getNode(uint8_t nodeId)
    {
        auto node = g_nodes.find(nodeId);
        if(node != g_nodes.end()){
            return node->second;
        } else {
            return nullptr;
        }
    }

    Node* getNode(OpenZWave::Notification const* _notification)
    {
        uint8_t const nodeId = _notification->GetNodeId();
        return getNode(nodeId);
    }

    void setValue(uint8_t nodeId, bool value)
    {
        g_criticalSection.lock();
        Node * node = getNode(nodeId);
        if(node == nullptr)
            return;
        node->setValue(value, 0x25);
        g_criticalSection.unlock();
    }

    void setValue(uint8_t nodeId, uint8_t value)
    {
        g_criticalSection.lock();
        Node * node = getNode(nodeId);
        if(node == nullptr)
            return;
        node->setValue(value, 0x25);
        g_criticalSection.unlock();
    }
}
