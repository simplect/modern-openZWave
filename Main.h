#ifndef __MAIN_HEADER__
#define __MAIN_HEADER__


#include <string>
#include <list>
#include <mutex>
#include <vector>
#include "ValueStore.h"
#include "Notification.h"
#include "MNode.h"

namespace Modernozw {
    uint32_t g_homeId = 0;
    bool g_initFailed = false;
    bool g_runMainThread = true; //If set to false it will break the main loop
    uint8_t g_pollTime = 10; //Amount of seconds delay between each poll
    std::map <uint8_t, Node*> g_nodes;
    std::recursive_mutex g_criticalSection;
    std::condition_variable g_condInit;

}
//Threads

    void mainThread();
    void controlThread();
    //Hooks hooked to openzwave
    void onNotification(OpenZWave::Notification const* _notification, void* _context);

#endif
