/*
 * StateMachine.cpp
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#include <assert.h>
#include "StateMachine.h"

StateMachine::StateMachine(int maxStates) :
    _maxStates(maxStates),
    currentState(0),
	lastState(0),
	evtSrcExternal(false),
    _eventGenerated(false),
    _pEventData(NULL)
{
}

StateMachine::~StateMachine(){
	_stateMachineThread.join();
}

void StateMachine::start() {
	_stateMachineThread(StateThreadFunc);
}

// generates an external event. called once per external event
// to start the state machine executing
void StateMachine::ExternalEvent(unsigned char newState,
                                 EventData* pData)
{
    // if we are supposed to ignore this event
    if (newState == EVENT_IGNORED) {
        // just delete the event data, if any
        if (pData)
            delete pData;
    }
    else {
    	std::lock_guard<std::mutex> lock(_eventMutex);
        // generate the event and execute the state engine
        InternalEvent(newState, pData);
        //StateEngine();
    }
}

// generates an internal event. called from within a state
// function to transition to a new state
void StateMachine::InternalEvent(unsigned char newState,
                                 EventData* pData)
{
    _pEventData = pData;
    _eventGenerated = true;
    lastState = currentState;
    currentState = newState;
}

// the state engine executes the state machine states
void StateMachine::StateEngine(void)
{
    EventData* pDataTemp = NULL;

    // TBD - lock semaphore here
    std::lock_guard<std::mutex> lock(_eventMutex);

    // while events are being generated keep executing states
    if (_eventGenerated) {
        pDataTemp = _pEventData;  // copy of event data pointer
        _pEventData = NULL;       // event data used up, reset ptr
        _eventGenerated = false;  // event used up, reset flag

        assert(currentState < _maxStates);

        // execute the state passing in event data, if any
        const StateStruct* pStateMap = GetStateMap();
        (this->*pStateMap[currentState].pStateFunc)(pDataTemp);

        // if event data was used, then delete it
        if (pDataTemp) {
            delete pDataTemp;
            pDataTemp = NULL;
        }
    }
    // TBD - unlock semaphore here
}

void StateMachine::StateThreadFunc(void){
	assert(currentState == 0);

	//TODO add a break condition
	while(1){
		StateEngine();
	}
}
