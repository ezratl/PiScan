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
#include "threadname.h"
#include "SystemList.h"
#include "Entry.h"
#include "RadioSystem.h"
#include "request.h"
#include "Configuration.h"


#define DELAY_TIMEOUT	2.0

#define SCANNER_THREAD_NAME	"Scanner"

using namespace piscan;
using namespace std;

ScannerSM::ScannerSM(piscan::scan::SystemList &dataSource) : StateMachine(7), _systems(dataSource), _externalHold(false), _manualMode(false)
{
}

void ScannerSM::startScanner(){
	LOG_F(1, "Loading saved scanner state");
	piscan::config::ScannerState& state = app::system::getState().getScannerState();
	piscan::scan::EntryPtr entry;
	switch(state.scanState){
	case SCAN_STATE_HOLD:
		LOG_F(1, "Previous state hold");
		entry = _systems.getEntryByIndex(state.holdIndex);
		if(entry != nullptr && (entry->key() == state.holdKey))
			holdScan(state.holdIndex);
		else{
			LOG_F(1, "Invalid entry index, defaulting to scan");
			startScan();
		}
		break;
	case SCAN_STATE_MANUAL:
		LOG_F(1, "Previous state manual entry");
		manualEntry(new app::ManualEntryData(state.manualFreq, state.manualModualtion));
		break;
	case SCAN_STATE_SCAN:
	default:
		startScan();
	}
}

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

void ScannerSM::holdScan(vector<int> index){
	_externalHold.store(true);
	{
		lock_guard<mutex> lock(_holdMutex);
		_holdIndex = index;
	}
	LOG_F(1, "ExtEvent: holdScan");
	BEGIN_TRANSITION_MAP
		TRANSITION_MAP_ENTRY(ST_HOLD)
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

void ScannerSM::manualEntry(app::ManualEntryData* freq){
	LOG_F(1, "ExtEvent: manualEntry");
	BEGIN_TRANSITION_MAP
		TRANSITION_MAP_ENTRY(ST_MANUAL)
		TRANSITION_MAP_ENTRY(ST_MANUAL)
		TRANSITION_MAP_ENTRY(ST_MANUAL)
		TRANSITION_MAP_ENTRY(ST_MANUAL)
		TRANSITION_MAP_ENTRY(ST_MANUAL)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
	END_TRANSITION_MAP(new EventData(freq))
}

void ScannerSM::ST_Load(EventData* /* data */){
	setThreadName(SCANNER_THREAD_NAME);
	DLOG_F(9, "ST_Load");
	_systems.populateFromFile();
	LOG_F(INFO, "Loaded %lu systems", _systems.size());

	//_currentSystem = _systems[0];
	_systems.sortBins(app::demod::getTunerSampleRate());

	// do not issue event - SM will wait until an event is generated before proceeding
	LOG_F(1, "ScannerSM ready");
	notifyReady();
}

void ScannerSM::ST_Scan(EventData* /* data */){
	DLOG_F(9, "ST_Scan");
	if(currentState != lastState){
		_squelchHits = 0;
		DLOG_F(6, "State change: %i -> %i", lastState, currentState);
	}

	if(_systems.size() == 0){
		LOG_F(INFO, "Database is empty, no entries to scan - defaulting to manual entry");
		InternalEvent(ST_MANUAL, new EventData(new uint32_t(100000000))); 
		return;
	}

	_enableAudioOut(false);
	_currentContext.state = piscan::server::context::ScannerContext::SCAN;
	_manualMode = false;
	_externalHold = false;

	if(currentState != lastState){
		_broadcastContextUpdate();
	}

	if (!_squelchHits || (currentState != lastState)) {
		_currentEntry = _systems.getNextEntry();
		DLOG_F(7, (_currentEntry->isDummy() ? "retune %lli" : "getNextEntry %lli"), _currentEntry->freq());
	}

	if (_currentEntry->hasSignal()) {
		_squelchHits++;
		if (_squelchHits >= SQUELCH_TRIGGER_HITS) {
			LOG_F(2, "Signal found: %s", _currentEntry->tag().c_str());
			InternalEvent(ST_RECEIVE);
		} else {
			InternalEvent(ST_SCAN);
		}
	} else /*if(!evtSrcExternal)*/{
		_squelchHits = 0;
		InternalEvent(ST_SCAN);
	}

}

void ScannerSM::ST_Hold(EventData* /* data */){
	DLOG_F(9, "ST_Hold");
	if(currentState != lastState)
		DLOG_F(6, "State change: %i -> %i", lastState, currentState);

	bool indexHold = false;

	{
		lock_guard < mutex > lock(_holdMutex);
		if (_externalHold.load() && _holdIndex.size() > 0) {
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
	_currentContext.state = piscan::server::context::ScannerContext::HOLD;
	if(currentState != lastState || indexHold)
		_broadcastContextUpdate();

	DLOG_F(8, "Ext hold: %i", _externalHold.load());

	/* start receive if signal active */
	if (_currentEntry->hasSignal()) {
		LOG_F(5, "Receiving signal");
		InternalEvent(ST_RECEIVE);
	}
	/* stay in hold if state was triggered externally */
	else if(_externalHold.load()){
		InternalEvent(ST_HOLD);
	}
	/* check if entry was locked while holding */
	else if(_currentEntry->isLockedOut()){
		LOG_F(3, "Locked out during hold, resume scan");
		InternalEvent(ST_SCAN);
	}
	/* check timeout counter if entry has resume delay enabled */
	else if(_currentEntry->delayMS()){
		auto current = time_point_cast<milliseconds>(system_clock::now());

		if(std::chrono::duration_cast<std::chrono::duration<int, std::milli>>(current- timeoutStart).count() < _currentEntry->delayMS()){
			InternalEvent(ST_HOLD);
		}
		else if(!evtSrcExternal){
			LOG_F(3, "Delay timed out, resuming scan");
			InternalEvent(ST_SCAN);
		}
	}
	/* entry does not have delay and hold was internal */
	else if(!evtSrcExternal){
		LOG_F(3, "Resuming scan");
		InternalEvent(ST_SCAN);
	}

	/* wait for 1ms */
	usleep(1000);
}

void ScannerSM::ST_Receive(EventData* /* data */){
	DLOG_F(9, "ST_Receive");
	if(currentState != lastState)
		DLOG_F(6, "State change: %i -> %i", lastState, currentState);

	/* check if entry was locked while holding */
	if (_currentEntry->isLockedOut()) {
		LOG_F(3, "Locked out during hold, resume scan");
		InternalEvent(ST_SCAN);
	}

	_enableAudioOut(true);
	_currentContext.state = piscan::server::context::ScannerContext::RECEIVE;
	if(currentState != lastState)
		_broadcastContextUpdate();

	if (_currentEntry->hasSignal()) {
		InternalEvent(ST_RECEIVE);
	}
	else{
		LOG_F(5, "Signal lost");
		InternalEvent(ST_HOLD);
		timeoutStart = time_point_cast<milliseconds>(system_clock::now());
		return;
	}

	/* wait for 1ms */
	usleep(1000);
}

void ScannerSM::ST_Manual(EventData* data){
	DLOG_F(9, "ST_Manual");
	if(currentState != lastState)
		DLOG_F(6, "State change: %i -> %i", lastState, currentState);

	app::ManualEntryData* freq = reinterpret_cast<app::ManualEntryData*>(data->data);

	LOG_F(1, "Setting manual frequency to %.4lfMHz", (freq->freq / 1E6));

	/* delete old manual entry */
	//if(_manualEntry != nullptr)
		//delete _manualEntry;

	//TODO will replace with a function map probably
	if(freq->modulation == "FM" || freq->modulation == "NFM")
		_manualEntry = make_shared<piscan::scan::FMChannel>(freq->freq, "", false, 0);
	else if(freq->modulation == "AM")
		_manualEntry = make_shared<piscan::scan::AMChannel>(freq->freq, "", false, 0);
	else{
		LOG_F(WARNING, "Invalid manual entry modulation: %s", freq->modulation.c_str());
		_manualEntry = make_shared<piscan::scan::FMChannel>(freq->freq, "", false, 0);
	}
	delete freq;
	_currentEntry = _manualEntry;
	_externalHold = true;
	_manualMode = true;
	InternalEvent(ST_HOLD);
	
}

void ScannerSM::ST_SaveAll(EventData* /* data */){
	DLOG_F(9, "ST_SaveAll");
	LOG_F(1, "Saving state");
	piscan::config::ScannerState& state = app::system::getState().getScannerState();
	state.holdIndex = {};
	state.holdKey = "";
	state.manualFreq = 0;
	state.manualModualtion = "";

	if(_manualMode == true){
		state.scanState = SCAN_STATE_MANUAL;
		state.manualFreq = _currentEntry->freq();
		state.manualModualtion = _currentEntry->modulation();
	}
	else if (_externalHold == true){
		state.scanState = SCAN_STATE_HOLD;
		state.holdIndex.push_back(_currentEntry->getSysIndex());
		state.holdIndex.push_back(_currentEntry->getEntryIndex());
		state.holdKey = _currentEntry->key();
	}
	else
		state.scanState = SCAN_STATE_SCAN;

	LOG_F(1, "Saving database");

	_systems.writeToFile();

	InternalEvent(ST_STOPPED);
}

void ScannerSM::ST_Stopped(EventData* /* data */){
	DLOG_F(9, "ST_Stopped");
	stop(false);
	LOG_F(1, "ScannerSM stopped");
	notifyDeinit();
}

void ScannerSM::_broadcastContextUpdate() {
	DLOG_F(6, "Broadcasting context");
	lock_guard<mutex> lock(_contextMutex);
	if (_currentContext.state != piscan::server::context::ScannerContext::SCAN)
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
			_currentContext.entryIndex = to_string(_currentEntry->getSysIndex()) + "-" + to_string(_currentEntry->getEntryIndex());
		}
		_currentContext.frequency = _currentEntry->freq();
		_currentContext.modulation = _currentEntry->modulation();
		_currentContext.delayMS = _currentEntry->delayMS();
		_currentContext.lockout = _currentEntry->isLockedOut();
	}
	else {
		_currentContext.clearFields();
	}
	
	app::server::scannerContextUpdate(_currentContext);
}

void ScannerSM::_enableAudioOut(bool en){
	app::demod::squelchBreak(en);
}

piscan::server::context::ScannerContext ScannerSM::getCurrentContext(){
	unique_lock<mutex> lock(_contextMutex);
	return piscan::server::context::ScannerContext(_currentContext);
}
