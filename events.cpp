#include "events.h"
#include <iostream>

// -----------------------------
//
// -----------------------------
Events::Events()
{
    spdlog::info(u8"Events contructor.");
    isEventsLoopRunning = false;
    eventsLoopThread = nullptr;
    spdlog::info(u8"Events class constructor.");
    needStop = false;
}
// -----------------------------
//
// -----------------------------
Events::~Events()
{
    spdlog::info(u8"Events destructor.");        
}
// -----------------------------
// 
// -----------------------------
void Events::eventsLoop(void)
{
    isEventsLoopRunning = true;
    spdlog::info(u8"Вход в eventsLoop.");
    while (!needStop)
    {
        //queue.wait();
        queue.process();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    spdlog::info(u8"Выход из цикла обработки событий.");    
    isEventsLoopRunning = false;
}
// -----------------------------
// 
// -----------------------------
void Events::Run(void)
{
    spdlog::info(u8"Environment loop starting.");
    eventsLoopThread = new std::thread(&Events::eventsLoop, this);
    isEventsLoopRunning = true;
    needStop = false;
}
// -----------------------------
// 
// -----------------------------
void Events::Stop(void)
{
    needStop = true;
    if (eventsLoopThread != nullptr)
    {
        if (eventsLoopThread->joinable())
        {
            eventsLoopThread->join();
        }

        delete eventsLoopThread;
        eventsLoopThread = nullptr;
    }
}