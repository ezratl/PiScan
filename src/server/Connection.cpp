/*
 * Connection.cpp
 *
 *  Created on: Feb 12, 2019
 *      Author: ezra
 */

#include "connection.h"
//#include "request.h"

void Connection::notifyDisconnected() {
	ClientRequest::RequestParams params = { .type = NOTIFY_DISCONNECTED };
	issueRequest(params);
}

int Connection::issueRequest(ClientRequest::RequestParams params, void* data) {
	ClientRequest* rq = new ClientRequest(_handle, params, data, this);
	int r = _serverManager->giveRequest(rq);
	switch (r) {
	case ServerInterface::RQ_DENIED:
		systemMessage(
				GeneralMessage(GeneralMessage::ERROR,
						"Request failed: denied"));
		break;
	case ServerInterface::RQ_INSUFFICIENT_PERMISSION:
		systemMessage(
				GeneralMessage(GeneralMessage::ERROR,
						"Request failed: insufficient permissions"));
		break;
	case ServerInterface::RQ_INVALID_HANDLE:
		systemMessage(
				GeneralMessage(GeneralMessage::ERROR,
						"Request failed: bad connection handle"));
		break;
	default:
		break;
	}
	return r;
}

int Connection::systemFunction(SystemFunction function) {
	ClientRequest::RequestParams params = { .type = SYSTEM_FUNCTION };
	switch (function) {
	case STOP:
		params.subType = SYSTEM_STOP;
		break;
	default:
		return -1;
	}
	return issueRequest(params);
}

int Connection::scannerFunction(ScannerFunction function, uint32_t freq) {
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
}

int Connection::setDemodSquelch(int level) {
	ClientRequest::RequestParams params = { .type = DEMOD_CONFIGURE, .subType =
			DEMOD_SET_SQUELCH };
	return issueRequest(params, new int(level));
}

int Connection::setDemodGain(int level) {
	ClientRequest::RequestParams params = { .type = DEMOD_CONFIGURE, .subType =
			DEMOD_SET_GAIN };
	return issueRequest(params, new int(level));
}

int Connection::getScannerContext() {
	ClientRequest::RequestParams params = { .type = GET_CONTEXT, .subType = SCANNER_CONTEXT };
	return issueRequest(params);
}

int Connection::getDemodContext(){
	ClientRequest::RequestParams params = { .type = GET_CONTEXT, .subType = DEMOD_CONTEXT };
	return issueRequest(params);
}
