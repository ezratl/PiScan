/*
 * ScannerStateMachine.cpp
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#include <unistd.h>
#include <mutex>

#include "PiScan.h"
#include "ScannerSM.h"
#include "ListGenerator.h"
#include "loguru.hpp"


#define DELAY_TIMEOUT	2.0

using namespace piscan;

ScannerSM::ScannerSM(MessageReceiver& central, SystemList& dataSource) :
		StateMachine(7), _centralQueue(central), _systems(dataSource), _externalHold(false), _manualMode(false) {
}

void ScannerSM::startScan(){
	_externalHold = false;
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

void ScannerSM::holdScan(std::vector<int> index){
	_externalHold = true;
	{
		std::lock_guard<std::mutex> lock(_holdMutex);
		_holdIndex = index;
	}
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

void ScannerSM::manualEntry(uint32_t* freq){
	LOG_F(1, "ExtEvent: manualEntry");
	BEGIN_TRANSITION_MAP
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
		TRANSITION_MAP_ENTRY(ST_MANUAL)
		TRANSITION_MAP_ENTRY(ST_MANUAL)
		TRANSITION_MAP_ENTRY(ST_MANUAL)
		TRANSITION_MAP_ENTRY(ST_MANUAL)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
	END_TRANSITION_MAP(new EventData(freq))
}

void ScannerSM::ST_Load(EventData* data){
	loguru::set_thread_name("Scanner");
	DLOG_F(9, "ST_Load");
	_systems.populateFromFile();
	LOG_F(INFO, "Loaded %u systems", _systems.size());

	//_currentSystem = _systems[0];
	_systems.sortBins(getTunerSampleRate());

	// do not issue event - SM will wait until an event is generated before proceeding
	//InternalEvent(ST_SCAN);
	//auto message = std::make_shared<ControllerMessage>(SCANNER_SM, ControllerMessage::NOTIFY_READY);
	//_centralQueue.giveMessage(message);
	LOG_F(1, "ScannerSM ready");
	notifyReady();
}

void ScannerSM::ST_Scan(EventData* data){
	DLOG_F(9, "ST_Scan");
	if(currentState != lastState){
		_squelchHits = 0;
		DLOG_F(6, "State change: %i -> %i", lastState, currentState);
	}

	_enableAudioOut(false);
	_currentContext.state = ScannerContext::SCAN;
	_manualMode = false;

	if(currentState != lastState)
		_broadcastContextUpdate();

	if (!_squelchHits || (currentState != lastState)) {

		
		_currentEntry = _systems.getNextEntry();

	}

	if (_currentEntry->hasSignal()) {
		_squelchHits++;
		if (_squelchHits >= SQUELCH_TRIGGER_HITS) {
			LOG_F(2, "Signal found: %s", _currentEntry->tag().c_str());
			InternalEvent(ST_RECEIVE);
		} else {
			InternalEvent(ST_SCAN);
		}
	} else {
		_squelchHits = 0;
		InternalEvent(ST_SCAN);
	}

}

void ScannerSM::ST_Hold(EventData* data){
	DLOG_F(9, "ST_Hold");
	if(currentState != lastState)
		DLOG_F(6, "State change: %i -> %i", lastState, currentState);

	bool indexHold = false;

	{
		std::lock_guard < std::mutex > lock(_holdMutex);
		if (_externalHold && _holdIndex.size() > 0) {
			_currentEntry = _systems.getEntryByIndex(_holdIndex);
			LOG_F(1, "Index hold");
			_holdIndex.clear();
			indexHold = true;
		}
	}

	/* don't hold on dummy channels */
	while(_currentEntry->isDummy()){
		_currentEntry->hasSignal();
		_currentEntry = _systems.getNextEntry();
	}

	_enableAudioOut(false);
	_currentContext.state = ScannerContext::HOLD;
	if(currentState != lastState || indexHold)
		_broadcastContextUpdate();

	/* start receive if signal active */
	if (_currentEntry->hasSignal()) {
		LOG_F(5, "Receiving signal");
		InternalEvent(ST_RECEIVE);
	}
	/* stay in hold if state was triggered externally */
	else if(_externalHold){
		InternalEvent(ST_HOLD);
	}
	/* check timeout counter if entry has resume delay enabled */
	else if(_currentEntry->delay() && timeoutStart != 0){
		if(std::difftime(std::time(nullptr), timeoutStart) < _currentEntry->delay()){
			InternalEvent(ST_HOLD);
		}
		else {
			LOG_F(3, "Delay timed out, resuming scan");
			InternalEvent(ST_SCAN);
		}
	}
	/* entry does not have delay and hold was internal */
	else{
		LOG_F(3, "Resuming scan");
		InternalEvent(ST_SCAN);
	}

	/* wait for 1ms */
	usleep(1000);
}

void ScannerSM::ST_Receive(EventData* data){
	DLOG_F(9, "ST_Receive");
	if(currentState != lastState)
		DLOG_F(6, "State change: %i -> %i", lastState, currentState);

	_enableAudioOut(true);
	_currentContext.state = ScannerContext::RECEIVE;
	if(currentState != lastState)
		_broadcastContextUpdate();

	if (_currentEntry->hasSignal()) {
		InternalEvent(ST_RECEIVE);
	}
	else{
		LOG_F(5, "Signal lost");
		InternalEvent(ST_HOLD);
		timeoutStart = std::time(nullptr);
		return;
	}

	/* wait for 1ms */
	usleep(1000);
}

void ScannerSM::ST_Manual(EventData* data){
	DLOG_F(9, "ST_Manual");
	if(currentState != lastState)
		DLOG_F(6, "State change: %i -> %i", lastState, currentState);

	uint32_t* freq = reinterpret_cast<uint32_t*>(data->data);

	LOG_F(1, "Setting manual frequency to %.4lfMHz", (*freq / 1E6));

	/* delete old manual entry */
	//if(_manualEntry != nullptr)
		//delete _manualEntry;

	_manualEntry = std::make_shared<FMChannel>(*freq, "", false, false);
	delete freq;
	_currentEntry = _manualEntry;
	_externalHold = true;
	_manualMode = true;
	InternalEvent(ST_HOLD);
	
}

void ScannerSM::ST_SaveAll(EventData* data){
	DLOG_F(9, "ST_SaveAll");
	LOG_F(1, "Saving database");

	_systems.writeToFile();

	InternalEvent(ST_STOPPED);
}

void ScannerSM::ST_Stopped(EventData* data){
	DLOG_F(9, "ST_Stopped");
	stop(false);
	//auto message = std::make_shared<ControllerMessage>(SCANNER_SM, ControllerMessage::NOTIFY_STOPPED);
	//_centralQueue.giveMessage(message);
	LOG_F(1, "ScannerSM stopped");
	notifyDeinit();
}

void ScannerSM::_broadcastContextUpdate() {
	DLOG_F(6, "Broadcasting context");
	std::lock_guard<std::mutex> lock(_contextMutex);
	if (_currentContext.state != ScannerContext::SCAN)
	{
		if (_manualMode)
		{
			_currentContext.systemTag = "Manual";
			_currentContext.entryTag = "Manual entry";
			_currentContext.entryIndex = "MAN";
		}
		else
		{
			//_currentContext.systemTag = _currentSystem->tag();
			_currentContext.systemTag = _systems[_currentEntry->getSysIndex()]->tag();
			_currentContext.entryTag = _currentEntry->tag();
			_currentContext.entryIndex = std::to_string(_currentEntry->getSysIndex()) + "-" + std::to_string(_currentEntry->getEntryIndex());
		}
		_currentContext.frequency = _currentEntry->freq();
		_currentContext.modulation = _currentEntry->modulation();
	}
	else {
		_currentContext.clearFields();
	}

	auto message = std::make_shared<ServerMessage>(SCANNER_SM, ServerMessage::CONTEXT_UPDATE, new ScannerContext(_currentContext));

	_centralQueue.giveMessage(message);
}

void ScannerSM::_enableAudioOut(bool en){
	/*Message* message;
	if(en){
		message = new AudioMessage(SCANNER_SM, AudioMessage::ENABLE_OUTPUT);
	}
	else{
		message = new AudioMessage(SCANNER_SM, AudioMessage::DISABLE_OUTPUT);
	}
	_centralQueue.giveMessage(*message);*/

	//TODO temporary
	//rtl_fm_mute((int)(!en));
	//auto message = std::make_shared<DemodMessage>(SCANNER_SM, DemodMessage::OPEN_AUDIO, (void*) !en);
	//_centralQueue.giveMessage(message);
	piscan::audioMute(en);
}

void ScannerSM::giveMessage(std::shared_ptr<Message> message) {
	auto msg = std::dynamic_pointer_cast<ScannerMessage>(message);

	DLOG_F(7, "Message rcv - src:%i | type:%i", msg->source, msg->type);

	switch (msg->type) {
	/* stop call */
	case ScannerMessage::STOP:
		stopScanner();
		break;

	/* handle client request */
	case ScannerMessage::CLIENT_REQUEST:
		_handleRequest(*(static_cast<ClientRequest*>(msg->pData)));
		break;
	/* handle external state trigger */
	case ScannerMessage::STATE_CHANGE:
		auto newState = static_cast<States>(reinterpret_cast<size_t>(msg->pData) & 0xFF);
		switch (newState) {
		case ScannerMessage::STATE_SCAN:
			startScan();
			break;
		case ScannerMessage::STATE_HOLD:
			holdScan();
			break;
		default:
			DLOG_F(WARNING, "Invalid state request");
			break;
		}
		break;

	}

}

void ScannerSM::_handleRequest(ClientRequest& request) {
	ClientRequest* rq = &request;
	if (rq->rqInfo.type == SCANNER_FUNCTION) {
		switch (rq->rqInfo.subType) {
		case SCANNER_STATE_SCAN:
			startScan();
			break;
		case SCANNER_STATE_HOLD:
			if(request.pData != nullptr){
				std::vector<int>* indexData = reinterpret_cast<std::vector<int>*>(request.pData);
				holdScan(*indexData);
				delete indexData;
			}
			else
				holdScan();
			break;
		case SCANNER_STATE_MANUAL:
			manualEntry(reinterpret_cast<uint32_t*>(rq->pData));
			break;
		default:
			break;
		}
	}
	else if (rq->rqInfo.type == GET_CONTEXT){
		std::unique_lock<std::mutex> lock(_contextMutex);
		ScannerContext* context = new ScannerContext(_currentContext);
		lock.unlock();
		rq->connection->scannerContextRequestCallback(rq->rqHandle, context);
	}
	else{
		DLOG_F(WARNING, "Invalid scanner request type: %i", rq->rqInfo.type);
	}

	delete rq;
}

ScannerContext ScannerSM::getCurrentContext(){
	std::unique_lock<std::mutex> lock(_contextMutex);
	return ScannerContext(_currentContext);
}
