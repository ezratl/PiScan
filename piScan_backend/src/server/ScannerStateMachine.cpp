/*
 * ScannerStateMachine.cpp
 *
 *  Created on: Feb 9, 2019
 *      Author: ezra
 */

#include "ScannerStateMachine.h"

ScannerStateMachine::ScannerStateMachine() :
		StateMachine(7), systems(), currentSystem(NULL), currentEntry(NULL) {
}

/*ScannerStateMachine::~ScannerStateMachine() {
	// TODO Auto-generated destructor stub
}*/

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
		TRANSITION_MAP_ENTRY(EVENT_IGNORED)
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

void ScannerStateMachine::ST_Load(){
	//file read and system tree population

	InternalEvent(ST_SCAN);
}

void ScannerStateMachine::ST_Scan(){

	if(currentEntry->hasSignal()){
		InternalEvent(ST_RECEIVE);
	}

}

void ScannerStateMachine::ST_Hold(){

	if (currentEntry->hasSignal()) {
		InternalEvent(ST_RECEIVE);
	}
}

void ScannerStateMachine::ST_Receive(){

	if (currentEntry->hasSignal()) {
		InternalEvent(ST_RECEIVE);
	}
	else{
		InternalEvent(ST_HOLD);
	}
}

void ScannerStateMachine::ST_Manual(){

}

void ScannerStateMachine::ST_SaveAll(){

	InternalEvent(ST_STOPPED);
}

void ScannerStateMachine::ST_Stopped(){

}
