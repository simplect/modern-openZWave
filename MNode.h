#ifndef _NODE_HEADER_
#define _NODE_HEADER_

#include "Main.h"

#include "ValueStore.h"
#include "Notification.h"

namespace modernozw {

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
            void high(const OpenZWave::Notification * _notification);
            void low(const OpenZWave::Notification * _notification);
            //Setters
            void setValue(uint8_t value);
            void setValue(bool value);
    };

       //Helpers
    Node * getNode(uint8_t nodeId);
    void setValue(uint8_t node, bool value);
    void setValue(uint8_t node, uint8_t value);
}
#endif
