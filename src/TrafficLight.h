#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;


// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
// Also, there should be an std::condition_variable as well as an std::mutex as private members. 

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