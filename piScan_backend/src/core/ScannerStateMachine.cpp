/*
 * ScannerStateMachine.cpp
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#include "ScannerStateMachine.h"

ScannerStateMachine::ScannerStateMachine(MessageReceiver& central, SystemList& dataSource) :
		StateMachine(7), _centralQueue(central), _systems(dataSource), _currentSystem(NULL), _currentEntry(NULL) {
}

//enum States {
//		ST_LOAD = 0,
//		ST_SCAN,
//		ST_HOLD,
//		ST_MANUAL,
//		ST_SAVEALL,
//		ST_STOPPED,
//	};

void ScannerStateMachine::startScan(){
	BEGIN_TRANSITION_MAP
		TRANSITION_MAP_ENTRY(ST_SCAN)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
		TRANSITION_MAP_ENTRY(ST_SCAN)
		TRANSITION_MAP_ENTRY(ST_SCAN)
		TRANSITION_MAP_ENTRY(ST_SCAN)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
	END_TRANSITION_MAP(NULL)
}

void ScannerStateMachine::holdScan(){
	BEGIN_TRANSITION_MAP
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
		TRANSITION_MAP_ENTRY(ST_HOLD)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
	END_TRANSITION_MAP(NULL)
}

void ScannerStateMachine::stopScanner(){
	BEGIN_TRANSITION_MAP
		TRANSITION_MAP_ENTRY(ST_SAVEALL)
		TRANSITION_MAP_ENTRY(ST_SAVEALL)
		TRANSITION_MAP_ENTRY(ST_SAVEALL)
		TRANSITION_MAP_ENTRY(ST_SAVEALL)
		TRANSITION_MAP_ENTRY(ST_SAVEALL)
		TRANSITION_MAP_ENTRY(ST_SAVEALL)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
	END_TRANSITION_MAP(NULL)
}

void ScannerStateMachine::ST_Load(EventData* data){
	//file read and system tree population

	// do not issue event - SM will wait until an event is generated before proceeding
	//InternalEvent(ST_SCAN);
	Message& message = new ControllerMessage(SCANNER_SM, ControllerMessage::NOTIFY_READY);
	_centralQueue.giveMessage(message);
}

void ScannerStateMachine::ST_Scan(EventData* data){
	_enableAudioOut(false);

	// incremental scan pattern
	_entryCounter = (_entryCounter + 1) % _currentSystem.size();

	if(_entryCounter == 0){
		assert(_systems != NULL);
		_sysCounter = (_sysCounter + 1) % _systems.size();

		_currentSystem = _systems[_sysCounter];
		assert(_currentSystem != NULL);

		_broadcastSystemContext(_currentSystem);
	}

	_currentEntry = _currentSystem[_entryCounter];
	assert(_currentEntry != NULL);

	if(_currentEntry.hasSignal()){
		InternalEvent(ST_RECEIVE);
	}

}

void ScannerStateMachine::ST_Hold(EventData* data){
	_enableAudioOut(false);
	_broadcastEntryContext(_currentSystem, _currentEntry);

	if (_currentEntry.hasSignal()) {
		InternalEvent(ST_RECEIVE);
	}
}

void ScannerStateMachine::ST_Receive(EventData* data){
	_enableAudioOut(true);
	_broadcastEntryContext(_currentSystem, _currentEntry);

	if (_currentEntry.hasSignal()) {
		InternalEvent(ST_RECEIVE);
	}
	else{
		InternalEvent(ST_HOLD);
	}
}

void ScannerStateMachine::ST_Manual(EventData* data){
	//TODO state for later implementation
}

void ScannerStateMachine::ST_SaveAll(EventData* data){

	InternalEvent(ST_STOPPED);
}

void ScannerStateMachine::ST_Stopped(EventData* data){
	Message& message = new ControllerMessage(SCANNER_SM, ControllerMessage::NOTIFY_STOPPED);
	_centralQueue.giveMessage(message);
}

void ScannerStateMachine::_broadcastSystemContext(RadioSystem& sys){
	assert(sys != NULL);

	_currentContext.system = currentState;
	_currentContext.system = sys;
	_currentContext.entry = NULL;
	Message& message = new ServerMessage(SCANNER_SM, ServerMessage::CONTEXT_UPDATE, &_currentContext);
	_centralQueue.giveMessage(message);
}

void ScannerStateMachine::_broadcastEntryContext(RadioSystem& sys, Entry& entry){
	assert(sys != NULL);
	assert(entry != NULL);

	_currentContext.system = currentState;
	_currentContext.system = sys;
	_currentContext.entry = entry;
	Message& message = new ServerMessage(SCANNER_SM, ServerMessage::CONTEXT_UPDATE, &_currentContext);
	_centralQueue.giveMessage(message);
}

void ScannerStateMachine::_enableAudioOut(bool en){
	Message& message;
	if(en){
		message = new AudioMessage(SCANNER_SM, AudioMessage::ENABLE_OUTPUT);
	}
	else{
		message = new AudioMessage(SCANNER_SM, AudioMessage::DISABLE_OUTPUT);
	}
	_centralQueue.giveMessage(message);
}

void ScannerStateMachine::giveMessage(Message& message) {
	assert(message != NULL);
	auto msg = dynamic_cast<ScannerMessage&>(message);

	switch (msg.type) {
	/* handle client request */
	case ScannerMessage::CLIENT_REQUEST:

		break;
	/* handle external state trigger */
	case ScannerMessage::STATE_CHANGE:
		auto newState = reinterpret_cast<unsigned char>(msg.pData);
		switch (newState) {
		case ScannerMessage::STATE_SCAN:
			startScan();
			break;
		case ScannerMessage::STATE_HOLD:
			holdScan();
			break;
		}
		break;
	/* stop call */
	case ScannerMessage::STOP:
		stopScanner();
		break;
	}

	delete msg;
}
