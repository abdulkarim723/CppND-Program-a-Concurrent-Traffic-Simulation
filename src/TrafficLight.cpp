#include <iostream>
#include <random>
#include "TrafficLight.h"
#include <thread>

/* Implementation of class "MessageQueue" */

 
template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> lck(_mutex);
    _con.wait(lck, [this](){return !_queue.empty();});
    T _m = std::move(_queue.back());
    _queue.pop_back();
    return _m;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> lck_(_mutex);
    _queue.emplace_back(msg);
    _con.notify_one();
}


/* Implementation of class "TrafficLight" */


TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while (true)
    {
        if(_messages.receive() == TrafficLightPhase::green)
            return;
    }
    
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    int minCycleDuration = 4;
    int maxCycleDuration = 6;
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;
    lastUpdate = std::chrono::system_clock::now();
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if(timeSinceLastUpdate>=minCycleDuration && timeSinceLastUpdate<=maxCycleDuration){
            // toggle between red and green
            if(_currentPhase == TrafficLightPhase::green) _currentPhase = TrafficLightPhase::red;
            else _currentPhase = TrafficLightPhase::green;
            _messages.send(std::move(_currentPhase));
            lastUpdate = std::chrono::system_clock::now();
        }
    }

}

