/*
 * ScannerStateMachine.h
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#ifndef SERVER_SCANNERSTATEMACHINE_H_
#define SERVER_SCANNERSTATEMACHINE_H_

#include <ctime>

#include "StateMachine.h"
#include "SystemList.h"
#include "Entry.h"
#include "messages.h"
#include "clientmessage.h"

#define SQUELCH_TRIGGER_HITS	25

namespace piscan {

class ScannerSM: public MessageReceiver, public StateMachine {
public:
	ScannerSM(MessageReceiver& central, SystemList& dataSource);
	~ScannerSM() {};

	void startScan();
	void holdScan(std::vector<int> index = std::vector<int>());
	void stopScanner();
	void manualEntry(uint32_t* freq);
	void giveMessage(std::shared_ptr<Message> message);
private:
	void ST_Load(EventData* data);
	void ST_Scan(EventData* data);
	void ST_Hold(EventData* data);
	void ST_Receive(EventData* data);
	void ST_Manual(EventData* data);
	void ST_SaveAll(EventData* data);
	void ST_Stopped(EventData* data);

	BEGIN_STATE_MAP
		STATE_MAP_ENTRY(&ScannerSM::ST_Load)
		STATE_MAP_ENTRY(&ScannerSM::ST_Scan)
		STATE_MAP_ENTRY(&ScannerSM::ST_Hold)
		STATE_MAP_ENTRY(&ScannerSM::ST_Receive)
		STATE_MAP_ENTRY(&ScannerSM::ST_Manual)
		STATE_MAP_ENTRY(&ScannerSM::ST_SaveAll)
		STATE_MAP_ENTRY(&ScannerSM::ST_Stopped)
	END_STATE_MAP

	enum States {
		ST_LOAD = 0,
		ST_SCAN,
		ST_HOLD,
		ST_RECEIVE,
		ST_MANUAL,
		ST_SAVEALL,
		ST_STOPPED,

		ST_INVALID = 255
	};

private:
	MessageReceiver& _centralQueue;
	//moodycamel::ReaderWriterQueue<Message> _msgQueue;
	SystemList& _systems;
	//RadioSystem* _currentSystem;
	std::shared_ptr<Entry> _currentEntry;
	std::shared_ptr<Entry> _manualEntry;
	//size_t _sysCounter = 0, _entryCounter = 0;
	ScannerContext _currentContext;
	std::mutex _contextMutex;

	std::atomic_bool _externalHold;
	std::atomic_bool _manualMode;
	std::mutex _holdMutex;
	std::vector<int> _holdIndex;
	std::time_t timeoutStart = 0;

	int _squelchHits = 0;

	void _broadcastContextUpdate();
	void _enableAudioOut(bool en);
	void _handleRequest(ClientRequest& request);

};

}

#endif /* SERVER_SCANNERSTATEMACHINE_H_ */
