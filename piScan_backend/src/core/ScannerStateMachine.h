/*
 * ScannerStateMachine.h
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#ifndef SERVER_SCANNERSTATEMACHINE_H_
#define SERVER_SCANNERSTATEMACHINE_H_

#include "StateMachine.h"
#include "SystemList.h"
#include "Entry.h"
#include "messages.h"


class ScannerStateMachine: public MessageReceiver, StateMachine {
public:
	ScannerStateMachine(MessageReceiver& central, SystemList& dataSource);
	//~ScannerStateMachine();

	void startScan();
	void holdScan();
	void stopScanner();
	void giveMessage(Message& message);
private:
	void ST_Load();
	void ST_Scan();
	void ST_Hold();
	void ST_Receive();
	void ST_Manual();
	void ST_SaveAll();
	void ST_Stopped();

	BEGIN_STATE_MAP
		STATE_MAP_ENTRY(ST_Load)
		STATE_MAP_ENTRY(ST_Scan)
		STATE_MAP_ENTRY(ST_Hold)
		STATE_MAP_ENTRY(ST_Receive)
		STATE_MAP_ENTRY(ST_Manual)
		STATE_MAP_ENTRY(ST_SaveAll)
		STATE_MAP_ENTRY(ST_Stopped)
	END_STATE_MAP

	enum States {
		ST_LOAD = 0,
		ST_SCAN,
		ST_HOLD,
		ST_RECEIVE,
		ST_MANUAL,
		ST_SAVEALL,
		ST_STOPPED,
	};

private:
	MessageReceiver& _centralQueue;
	//moodycamel::ReaderWriterQueue<Message> _msgQueue;
	SystemList& _systems;
	RadioSystem& _currentSystem;
	Entry& _currentEntry;
	size_t _sysCounter = 0, _entryCounter = 0;

	struct EntryContext {
		States state;
		const RadioSystem& system;
		const Entry& entry;
	};

	EntryContext _currentContext = {0, NULL, NULL};

	void _broadcastSystemContext(RadioSystem& sys);
	void _broadcastEntryContext(RadioSystem& sys, Entry& entry);
	void _enableAudioOut(bool en);

};

#endif /* SERVER_SCANNERSTATEMACHINE_H_ */
