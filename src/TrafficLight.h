#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;


template <class T>
class MessageQueue
{
public:
void send(T&& msg);
T receive();
private:
std::deque<T> _queue;
std::condition_variable _con;
std::mutex _mutex;
};

enum class TrafficLightPhase {red, green};

class TrafficLight : public TrafficObject
{
public:
    TrafficLight();
    void waitForGreen();
    void simulate();
    
    TrafficLightPhase getCurrentPhase();
    
private:
    void cycleThroughPhases();
    MessageQueue<TrafficLightPhase> _messages;
    std::condition_variable _condition;
    std::mutex _mutex;
    TrafficLightPhase _currentPhase;
};

#endif