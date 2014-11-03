// --------------------------------------------------------------------------
// main.h
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

#ifndef __MAIN_HEADER__
#define __MAIN_HEADER__


#include <string>
#include <list>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "ValueStore.h"
#include "Notification.h"
#include "MNode.h"

namespace Modernozw {
    uint32_t g_homeId = 0;
    bool g_initFailed = false;
    bool g_runMainThread = true; //If set to false it will break the main loop
    uint8_t g_pollTime = 30; //Amount of seconds delay between each poll
    std::map <uint8_t, Node*> g_nodes;
    std::recursive_mutex g_criticalSection;
    std::condition_variable g_condInit;

}
//Threads
int start(std::string = "/dev/ttyUSB0");
void mainThread();
void controlThread();
//Hooks hooked to openzwave
void onNotification(OpenZWave::Notification const* _notification, void* _context);

#endif
