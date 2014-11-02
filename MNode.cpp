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
#include "ValueStore.h"
#include "Notification.h"
#include "Manager.h"

#include "MNode.h"
#include "Main.h"

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

    void Node::high(const OpenZWave::Notification * _notification)
    {

    }

    void Node::low(const OpenZWave::Notification * _notification)
    {

    }

    void Node::setValue(uint8_t value)
    {
        using namespace OpenZWave;
        for (auto it = m_values.begin(); it != m_values.end(); ++it) {
            ValueID valueId = *it;
            if (valueId.GetCommandClassId() == 0x25) {
                Manager::Get()->SetValue(valueId, true);
                break;
            }
        }
    }

    void Node::setValue(bool value)
    {
        using namespace OpenZWave;
        for (auto it = m_values.begin(); it != m_values.end(); ++it) {
            ValueID valueId = *it;
            if (valueId.GetCommandClassId() == 0x25) {
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
        node->setValue(value);
        g_criticalSection.unlock();
    }

    void setValue(uint8_t nodeId, uint8_t value)
    {
        g_criticalSection.lock();
        Node * node = getNode(nodeId);
            return;
        node->setValue(value);
        g_criticalSection.unlock();
    }
}
