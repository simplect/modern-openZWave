// --------------------------------------------------------------------------
// MNode.h
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

#ifndef _NODE_HEADER_
#define _NODE_HEADER_

#include <list>
#include "ValueStore.h"
#include "Notification.h"

namespace Modernozw {

    class Node {
        private:
            const uint8_t m_high = 255;
            const uint8_t m_low = 0;
        public:
            uint8_t m_nodeId;
            uint32_t m_homeId;
            bool m_polled = false;
            std::list<OpenZWave::ValueID> m_values;

            Node(uint8_t m_nodeId, uint32_t homeId);
            Node(uint8_t m_nodeId);

            //Hooks which are called by the notification thread
            void nodeEvent(const OpenZWave::Notification *notification);
            void valueChanged(const OpenZWave::Notification *notification);
            //Setters
            void setValue(int32_t value, uint8_t commandClassId);
            void setValue(bool value, uint8_t commandClassId);
    };

       //Helpers
    Node * getNode(uint8_t nodeId);
    void setValue(uint8_t node, bool value);
    void setValue(uint8_t node, uint8_t value);
}
#endif
