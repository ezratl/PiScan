/*
 * ScannerStateMachine.cpp
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#include <ScannerSM.h>
#include "loguru.hpp"

#define DELAY_TIMEOUT	2.0

ScannerSM::ScannerSM(MessageReceiver& central, SystemList& dataSource) :
		StateMachine(7), _centralQueue(central), _systems(dataSource), _currentSystem(nullptr), _currentEntry(nullptr) {
}

//enum States {
//		ST_LOAD = 0,
//		ST_SCAN,
//		ST_HOLD,
//		ST_MANUAL,
//		ST_SAVEALL,
//		ST_STOPPED,
//	};

void ScannerSM::startScan(){
	LOG_F(1, "ExtEvent: startScan");
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

void ScannerSM::holdScan(){
	_externalHold = true;
	LOG_F(1, "ExtEvent: holdScan");
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

void ScannerSM::stopScanner(){
	LOG_F(1, "ExtEvent: stopScanner");
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

void ScannerSM::ST_Load(EventData* data){
	DLOG_F(9, "ST_Load");
	//file read and system tree population

	// do not issue event - SM will wait until an event is generated before proceeding
	//InternalEvent(ST_SCAN);
	Message* message = new ControllerMessage(SCANNER_SM, ControllerMessage::NOTIFY_READY);
	_centralQueue.giveMessage(*message);
	LOG_F(1, "ScannerSM ready");
}

void ScannerSM::ST_Scan(EventData* data){
	DLOG_F(9, "ST_Scan");
	_enableAudioOut(false);

	// incremental scan pattern
	_entryCounter = (_entryCounter + 1) % _currentSystem->size();

	if(_entryCounter == 0){
		//assert(_systems != NULL);
		_sysCounter = (_sysCounter + 1) % _systems.size();

		_currentSystem = _systems[_sysCounter];
		assert(_currentSystem != NULL);

		_broadcastSystemContext(_currentSystem);
	}

	_currentEntry = _currentSystem->operator[](_entryCounter);
	assert(_currentEntry != NULL);

	if(_currentEntry->hasSignal()){
		InternalEvent(ST_RECEIVE);
	}

}

void ScannerSM::ST_Hold(EventData* data){
	DLOG_F(9, "ST_Hold");
	_enableAudioOut(false);
	_broadcastEntryContext(_currentSystem, _currentEntry);

	/* start receive if signal active */
	if (_currentEntry->hasSignal()) {
		LOG_F(1, "Receiving signal");
		InternalEvent(ST_RECEIVE);
	}
	/* stay in hold if state was triggered externally */
	else if(_externalHold){
		InternalEvent(ST_HOLD);
	}
	/* check timeout counter if entry has resume delay enabled */
	else if(_currentEntry->useDelay() && timeoutStart != 0){
		if(std::difftime(std::time(nullptr), timeoutStart) < DELAY_TIMEOUT){
			InternalEvent(ST_HOLD);
		}
		else {
			LOG_F(1, "Delay timed out, resuming scan");
			InternalEvent(ST_SCAN);
		}
	}
	/* entry does not have delay and hold was internal */
	else{
		LOG_F(1, "Resuming scan");
		InternalEvent(ST_SCAN);
	}

}

void ScannerSM::ST_Receive(EventData* data){
	DLOG_F(9, "ST_Receive");
	_enableAudioOut(true);
	_broadcastEntryContext(_currentSystem, _currentEntry);

	if (_currentEntry->hasSignal()) {
		InternalEvent(ST_RECEIVE);
	}
	else{
		LOG_F(1, "Signal lost");
		InternalEvent(ST_HOLD);
		timeoutStart = std::time(nullptr);
	}
}

void ScannerSM::ST_Manual(EventData* data){
	//TODO state for later implementation
}

void ScannerSM::ST_SaveAll(EventData* data){
	DLOG_F(9, "ST_SaveAll");
	LOG_F(1, "Saving database");

	InternalEvent(ST_STOPPED);
}

void ScannerSM::ST_Stopped(EventData* data){
	DLOG_F(9, "ST_Stopped");
	stop(false);
	Message* message = new ControllerMessage(SCANNER_SM, ControllerMessage::NOTIFY_STOPPED);
	_centralQueue.giveMessage(*message);
	LOG_F(1, "ScannerSM stopped");
}

void ScannerSM::_broadcastSystemContext(RadioSystem* sys){
	assert(sys != NULL);
	//TODO not thread safe
	_currentContext.state = static_cast<States>(currentState);
	_currentContext.system = sys;
	_currentContext.entry = nullptr;
	Message* message = new ServerMessage(SCANNER_SM, ServerMessage::CONTEXT_UPDATE, &_currentContext);
	_centralQueue.giveMessage(*message);
}

void ScannerSM::_broadcastEntryContext(RadioSystem* sys, Entry* entry){
	assert(sys != NULL);
	assert(entry != NULL);
	//TODO not thread safe
	_currentContext.state = static_cast<States>(currentState);
	_currentContext.system = sys;
	_currentContext.entry = entry;
	Message* message = new ServerMessage(SCANNER_SM, ServerMessage::CONTEXT_UPDATE, &_currentContext);
	_centralQueue.giveMessage(*message);
}

void ScannerSM::_enableAudioOut(bool en){
	Message* message;
	if(en){
		message = new AudioMessage(SCANNER_SM, AudioMessage::ENABLE_OUTPUT);
	}
	else{
		message = new AudioMessage(SCANNER_SM, AudioMessage::DISABLE_OUTPUT);
	}
	_centralQueue.giveMessage(*message);
}

void ScannerSM::giveMessage(Message& message) {
	//assert(message != NULL);
	auto msg = dynamic_cast<ScannerMessage&>(message);

	switch (msg.type) {
	/* stop call */
	case ScannerMessage::STOP:
		stopScanner();
		break;

	/* handle client request */
	case ScannerMessage::CLIENT_REQUEST:

		break;
	/* handle external state trigger */
	case ScannerMessage::STATE_CHANGE:
		auto newState = static_cast<States>(reinterpret_cast<size_t>(msg.pData) & 0xFF);
		switch (newState) {
		case ScannerMessage::STATE_SCAN:
			startScan();
			break;
		case ScannerMessage::STATE_HOLD:
			holdScan();
			break;
		default:
			break;
		}
		break;

	}

	delete &msg;
}
