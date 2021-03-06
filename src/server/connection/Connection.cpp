/*
 * Connection.cpp
 *
 *  Created on: Feb 12, 2019
 *      Author: ezra
 */

#include "PiScan.h"
#include "connection.h"
#include "request.h"

namespace piscan {
namespace server {
namespace connection {

void Connection::notifyDisconnected() {
	ClientRequest::RequestParams params = { .type = NOTIFY_DISCONNECTED, .subType = -1 };
	issueRequest(params);
}

int Connection::issueRequest(ClientRequest::RequestParams params, void* data) {
	ClientRequest* rq = new ClientRequest(_handle, params, data, this);
	int r = _serverManager->giveRequest(rq);
	switch (r) {
	case ServerInterface::RQ_DENIED:
		handleSystemMessage(
				piscan::server::context::GeneralMessage(piscan::server::context::GeneralMessage::ERROR,
						"Request failed: denied"));
		break;
	case ServerInterface::RQ_INSUFFICIENT_PERMISSION:
		handleSystemMessage(
				piscan::server::context::GeneralMessage(piscan::server::context::GeneralMessage::ERROR,
						"Request failed: insufficient permissions"));
		break;
	case ServerInterface::RQ_INVALID_HANDLE:
		handleSystemMessage(
				piscan::server::context::GeneralMessage(piscan::server::context::GeneralMessage::ERROR,
						"Request failed: bad connection handle"));
		break;
	default:
		break;
	}
	return r;
}

int Connection::systemFunction(SystemFunction function) {
	ClientRequest::RequestParams params = { .type = SYSTEM_FUNCTION, .subType = -1 };
	switch (function) {
	case STOP:
		params.subType = SYSTEM_STOP;
		break;
	default:
		return -1;
	}
	return issueRequest(params);
}

/*int Connection::scannerFunction(ScannerFunction function, uint32_t freq) {
	ClientRequest::RequestParams params = { .type = SCANNER_FUNCTION };
	switch (function) {
	case SCAN:
		params.subType = SCANNER_STATE_SCAN;
		break;
	case HOLD:
		params.subType = SCANNER_STATE_HOLD;
		break;
	case MANUAL:
		params.subType = SCANNER_STATE_MANUAL;
		return issueRequest(params, new uint32_t(freq));
		break;
	default:
		return -1;
	}
	return issueRequest(params);
}*/

int Connection::scanStart() {
	//ClientRequest::RequestParams params = { .type = SCANNER_FUNCTION, .subType = SCANNER_STATE_SCAN };
	//return issueRequest(params);
	app::scanner::startScan();
	return 0;
}

int Connection::scanHold() {
	//ClientRequest::RequestParams params = { .type = SCANNER_FUNCTION, .subType = SCANNER_STATE_HOLD };
	//return issueRequest(params);
	app::scanner::holdScan();
	return 0;
}

int Connection::scanHoldEntry(std::vector<int> index) {
	//ClientRequest::RequestParams params = { .type = SCANNER_FUNCTION, .subType = SCANNER_STATE_HOLD };
	//return issueRequest(params, new std::vector<int>(index));
	app::scanner::holdScan(index);
	return 0;
}

int Connection::scanManualEntry(long freq, std::string mode) {
	//ClientRequest::RequestParams params = { .type = SCANNER_FUNCTION, .subType = SCANNER_STATE_MANUAL };
	//return issueRequest(params, new uint32_t(freq));3
	auto data = new app::ManualEntryData(freq, mode);
	app::scanner::manualEntry(data);
	return 0;
}

int Connection::setDemodSquelch(int level) {
	//ClientRequest::RequestParams params = { .type = DEMOD_CONFIGURE, .subType =
	//		DEMOD_SET_SQUELCH };
	//return issueRequest(params, new int(level));
	app::demod::setDemodSquelch(level);
	return 0;
}

int Connection::setDemodGain(float level) {
	//ClientRequest::RequestParams params = { .type = DEMOD_CONFIGURE, .subType =
	//		DEMOD_SET_GAIN };
	//return issueRequest(params, new int(level));
	app::demod::setTunerGain(level);
	return 0;
}

int Connection::getScannerContext() {
	//ClientRequest::RequestParams params = { .type = GET_CONTEXT, .subType = SCANNER_CONTEXT };
	//return issueRequest(params);
	contextUpdate(app::scanner::getScannerContext());
	return 0;
}

int Connection::getDemodContext(){
	//ClientRequest::RequestParams params = { .type = GET_CONTEXT, .subType = DEMOD_CONTEXT };
	//return issueRequest(params);
	contextUpdate(app::demod::getDemodContext());
	return 0;
}

int Connection::getSystemInfo(){
	handleSystemInfo(app::system::getSystemInfo());
	return 0;
}

}
}
}
