/*
 * StateMachine.h
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#ifndef SERVER_STATEMACHINE_H_
#define SERVER_STATEMACHINE_H_

#include <condition_variable>
#include <stdio.h>
#include <thread>
#include <mutex>
//#include "EventData.h"

namespace piscan {

class EventData
{
public:
	EventData(void* d) : data(d) {};
    virtual ~EventData() {};
    void* data;
};

struct StateStruct;

// base class for state machines
class StateMachine
{
public:
    StateMachine(int maxStates);
    virtual ~StateMachine() {
    	_run = false;
    	_stateMachineThread.join();
    }
    void start();
    void stop(bool);
protected:
    enum { EVENT_IGNORED = 0xFE, CANNOT_HAPPEN };
    unsigned char currentState;
    unsigned char lastState;
    bool evtSrcExternal;
    void ExternalEvent(unsigned char, EventData* = NULL);
    void InternalEvent(unsigned char, EventData* = NULL);
    virtual const StateStruct* GetStateMap() = 0;
    std::thread _stateMachineThread;
private:
    const int _maxStates;
    bool _eventGenerated;
    EventData* _pEventData;
    void StateEngine(void);
    void StateThreadFunc(void);
    
    std::mutex _eventMutex;
    std::condition_variable _cv;
    bool _run = false;
};

typedef void (StateMachine::*StateFunc)(EventData *);
struct StateStruct
{
    StateFunc pStateFunc;
};

#define BEGIN_STATE_MAP \
public:\
const StateStruct* GetStateMap() {\
    static const StateStruct StateMap[] = {

#define STATE_MAP_ENTRY(entry)\
    { static_cast<StateFunc>/*(static_cast<void(*)(EventData*)>*/(entry)/*)*/ },

#define END_STATE_MAP \
    { /*reinterpret_cast<StateFunc>*/(NULL) }\
    }; \
    return &StateMap[0]; }

#define BEGIN_TRANSITION_MAP \
    static const unsigned char TRANSITIONS[] = {\

#define TRANSITION_MAP_ENTRY(entry)\
    entry,

#define END_TRANSITION_MAP(data) \
    0 };\
    ExternalEvent(TRANSITIONS[currentState], data);

}
#endif /* SERVER_STATEMACHINE_H_ */
