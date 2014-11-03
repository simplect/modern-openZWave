// --------------------------------------------------------------------------
// main.cpp
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
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <vector>
#include <iostream>
#include <sys/resource.h>
#include <math.h>

#include "Main.h"
#include "MNode.cpp" //Todo: fix makefile so we can change this to .h
//openZWave imports
#include "Options.h"
#include "Manager.h"
#include "Driver.h"
#include "Group.h"
#include "Node.h"
#include "Notification.h"
#include "ValueStore.h"
#include "Value.h"
#include "ValueBool.h"

using namespace Modernozw;

int main(int argc, char *argv[])
{
    using namespace OpenZWave;
    std::cout << "Initializing OpenZWave" << std::endl;
    //Create the OpenZwave manager
    Options::Create("open-zwave/config/", "", "");
    Options::Get()->AddOptionBool("ConsoleOutput", false);
    Options::Get()->Lock();
    Manager::Create();

    //Add the event hook
    Manager::Get()->AddWatcher(onNotification, NULL);

    //Add the driver
    string port = (argc > 1) ? argv[1] : "/dev/ttyUSB0";
    Manager::Get()->AddDriver(port);

    //Wait for the manager and driver to complete initializing
    std::mutex mtx;
    std::unique_lock<std::mutex> mtxLock(mtx);
    g_condInit.wait(mtxLock);

    if(!g_initFailed){
        std::cout << "Sucessfully initialized OpenZWave" << std::endl;

        //Write the config
        Manager::Get()->WriteConfig(g_homeId);
        Driver::DriverData data;
        Manager::Get()->GetDriverStatistics(g_homeId, &data);

        //Start the threads
        std::thread firstThread(mainThread);
        std::thread secondThread(controlThread);
        secondThread.detach();
        firstThread.detach();

        //Watch the CPU usage of this process
        //if we hit 100% for a few seconds
        //then we might have a failing openZWave driver
        int iterations = 0;
        uint32_t numUsage = 0;
        uint32_t maxUsage = pow(10, 9);
        while(true){
            if(iterations == 4){
                //Hit the fourth time we got a billion on numUsage
                //Here we could implement a restarter or anything else
#if DEBUG
                std::cout << "Terminating, the openZWave driver got stuck"
                    << " in a loop and is wasting resources" << std::endl;
#endif
                break;
            }
            struct rusage usage;
            struct timeval start, end;
            getrusage(RUSAGE_SELF, &usage);
            start = usage.ru_utime;
            sleep(2);
            getrusage(RUSAGE_SELF, &usage);
            end = usage.ru_utime;
            numUsage = (end.tv_usec - start.tv_usec);
            if(numUsage > maxUsage){
                iterations++;
            }
        }
    }

    if(g_initFailed)
        return 1;
    else
        return 0;
}

void mainThread()
{
   int i = 0;
   while(g_runMainThread){
        i++;
        if(i >= g_pollTime){
            i = 0;
            for(auto &it : g_nodes){
                OpenZWave::Manager::Get()->RefreshNodeInfo(g_homeId, it.second->m_nodeId);
            }
        }
        sleep(1);
   }
}

void controlThread()
{
    std::string input = "";
    while(g_runMainThread){
        std::cin >> input;
        if(input == "exit")
            g_runMainThread = false;
        else if(input == "t")
            setValue(8, true);
        else if(input == "f")
            setValue(8, false);
    }
}

void onNotification(OpenZWave::Notification const* notification, void* context)
{
    using namespace OpenZWave;
    g_criticalSection.lock();

    switch(notification->GetType()){
        case Notification::Type_ValueAdded:
        {
            if(auto node = getNode(notification->GetNodeId())){
                node->m_values.push_back(notification->GetValueID());
            }
            break;
        }

        case Notification::Type_ValueRemoved:
        {
            if(auto node = getNode(notification->GetNodeId())){
                for(auto it = node->m_values.begin(); it != node->m_values.end(); ++it){
                    if((*it) == notification->GetValueID()){
                        node->m_values.erase(it);
                        break;
                    }
                }
            }
            break;
        }

        case Notification::Type_ValueChanged:
        {
            if(auto node = getNode(notification->GetNodeId())){
                ValueID value = notification->GetValueID();
                //Remove the old value
                for(auto it = node->m_values.begin(); it != node->m_values.end(); ++it){
                    if((*it) == notification->GetValueID()){
                        node->m_values.erase(it);
                        break;
                    }
                }
                node->m_values.push_back(value);
#if DEBUG
                std::string valueString = "";
                Manager::Get()->GetValueAsString(value, &valueString);
                std::cout << "Got a new value from node " << (int)node->m_nodeId
                    << " Name: " << Manager::Get()->GetValueLabel(value)
                    << " Value: " << valueString << std::endl;
#endif
                break;
            }
            break;
        }

        case Notification::Type_NodeAdded:
        {
            //Create a new node and add it with a id to the map
            Modernozw::Node *node = new Modernozw::Node(
                    notification->GetNodeId(), notification->GetHomeId());
            g_nodes.insert(std::make_pair(node->m_nodeId, node));
            break;
        }

        case Notification::Type_NodeRemoved:
        {
            //Remove the node from our list
            auto node = g_nodes.find(notification->GetNodeId());
            if(node != g_nodes.end()){
                delete &node;
                g_nodes.erase(node);
            }
            break;
        }

        case Notification::Type_NodeEvent:
        {
            if(auto node = getNode(notification->GetNodeId())){
                uint8_t value = notification->GetEvent();
#if DEBUG
                std::cout << "Got a new node event from node " << (int)node->m_nodeId
                    << " event value: " << (int)value << std::endl;
#endif
                if(value == 255){
                    node->high(notification);
                } else if(value == 0){
                    node->low(notification);
                }
            }
            break;
        }
        case Notification::Type_PollingEnabled:
        {
            if(auto node = getNode(notification->GetNodeId())){
                node->m_polled = true;
            }
            break;
        }
        case Notification::Type_PollingDisabled:
        {
            if(auto node = getNode(notification->GetNodeId())){
                node->m_polled = false;
            }
            break;
        }
        case Notification::Type_DriverReady:
        {
            g_homeId = notification->GetHomeId();
            break;
        }
        case Notification::Type_DriverFailed:
        {
            g_initFailed = true;
            g_condInit.notify_all();
            break;
        }
        case Notification::Type_AwakeNodesQueried:
        case Notification::Type_AllNodesQueried:
        {
            g_condInit.notify_all();
            break;
        }
        default:
        {

        }

    }
    g_criticalSection.unlock();
}
