#pragma once

#include <iostream>
#include "eventpp/eventqueue.h"
#include "eventpp/utilities/orderedqueuelist.h"
#include "spdlog/spdlog.h"
#include "spdlog/cfg/env.h" // support for loading levels from the environment variable
class MyEvent;
class MyPolicy;
using EQ = eventpp::EventQueue<int, void(std::shared_ptr<MyEvent>), MyPolicy>;
// -----------------------------
// 
// -----------------------------
// First let's define the event struct. e is the event type, priority determines the priority.
class MyEvent
{
public:
    int e;
    int priority=0;
    MyEvent(int e, int piority)
    {
        std::cout << "Event created" << std::endl;
        this->e = e;
        this->priority = priority;
    }
    ~MyEvent()
    {        
        std::cout << "Event destroyed" << std::endl;
    }
};

// The comparison function object used by eventpp::OrderedQueueList.
// The function compares the event by priority.
class MyCompare
{
public:
    template <typename T>
    bool operator() (const T& a, const T& b) const
    {
        return a.template getArgument<0>()->priority > b.template getArgument<0>()->priority;
    }
};

// Define the EventQueue policy
class MyPolicy
{
public:
    template <typename Item>
    using QueueList = eventpp::OrderedQueueList<Item, MyCompare >;

    static int getEvent(const MyEvent* event)
    {
        return event->e;
    }
};

// -----------------------------
// 
// -----------------------------
class Events
{
public:
    Events();
    ~Events();

    void Run(void);
    void Stop(void);
    bool isEventsLoopRunning;
    EQ queue;
private:
    bool needStop;
    void eventsLoop(void);
    std::thread* eventsLoopThread;
    
};
