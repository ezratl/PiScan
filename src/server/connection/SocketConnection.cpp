/*
 * SocketConnection.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: ezra
 */

#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <sys/wait.h>
#include <errno.h>

#include "loguru.hpp"
#include "SocketConnection.h"
#include "context.pb.h"
#include "messages.pb.h"
#include "connection/SocketConnection.h"

namespace piscan {
namespace server {
namespace connection {

bool SocketConnection::connect(){
	Connection::connect();
	_startRead();
	return true;
}

void SocketConnection::disconnect(){
	if (_socket.is_open()) {
		_socket.close();
		notifyDisconnected();
	}
}

void SocketConnection::contextUpdate(const piscan::server::context::ScannerContext context){
	auto ctx = new piscan_pb::ScannerContext();
	switch(context.state){
	case piscan::server::context::ScannerContext::ScannerState::OTHER_STATE:
		ctx->set_state(piscan_pb::ScannerContext_State_INVAL);
		break;
	case piscan::server::context::ScannerContext::ScannerState::SCAN:
		ctx->set_state(piscan_pb::ScannerContext_State_SCAN);
		break;
	case piscan::server::context::ScannerContext::ScannerState::HOLD:
		ctx->set_state(piscan_pb::ScannerContext_State_HOLD);
		break;
	case piscan::server::context::ScannerContext::ScannerState::RECEIVE:
		ctx->set_state(piscan_pb::ScannerContext_State_RECEIVE);
		break;
	default:
		ctx->set_state(piscan_pb::ScannerContext_State_INVAL);
	}

	auto entryData = new piscan_pb::Entry();

	entryData->set_freq(context.frequency);
	entryData->set_systemtag(context.systemTag);
	entryData->set_entrytag(context.entryTag);
	entryData->set_modulation(context.modulation);
	entryData->set_entryindex(context.entryIndex.c_str());

	ctx->set_allocated_entrydata(entryData);

	piscan_pb::ServerToClient msg;
	msg.set_type(piscan_pb::ServerToClient_Type_SCANNER_CONTEXT);
	msg.set_allocated_scannercontext(ctx);

	msg.SerializeToArray(_writeBuffer, WRITE_BUFFER_LENGTH);
	_startWrite(_writeBuffer, msg.ByteSize());
	//delete ctx; // its seems protobuf handles deletion, leaving this causes a double free error
}

void SocketConnection::contextUpdate(const piscan::server::context::DemodContext context){
	piscan_pb::DemodContext* ctx = new piscan_pb::DemodContext();

	ctx->set_gain(context.gain);
	ctx->set_squelch(context.squelch);

	piscan_pb::ServerToClient msg;
	msg.set_type(piscan_pb::ServerToClient_Type_DEMOD_CONTEXT);
	msg.set_allocated_demodcontext(ctx);

	msg.SerializeToArray(_writeBuffer, WRITE_BUFFER_LENGTH);
	_startWrite(_writeBuffer, msg.ByteSize());
	//delete ctx;
}

void SocketConnection::handleSystemMessage(const piscan::server::context::GeneralMessage message) {
	auto ctx = new piscan_pb::GeneralMessage();

	switch(message.type){
	case piscan::server::context::GeneralMessage::INFO:
		ctx->set_type(piscan_pb::GeneralMessage_Type_INFO);
		break;
	case piscan::server::context::GeneralMessage::WARNING:
		ctx->set_type(piscan_pb::GeneralMessage_Type_WARNING);
		break;
	case piscan::server::context::GeneralMessage::ERROR:
		ctx->set_type(piscan_pb::GeneralMessage_Type_ERROR);
		break;
	default:
		ctx->set_type(piscan_pb::GeneralMessage_Type_INFO);
		break;
	}

	ctx->set_content(message.content);

	piscan_pb::ServerToClient msg;
	msg.set_type(piscan_pb::ServerToClient_Type_GENERAL_MESSAGE);
	msg.set_allocated_generalmessage(ctx);

	msg.SerializeToArray(_writeBuffer, WRITE_BUFFER_LENGTH);
	_startWrite(_writeBuffer, msg.ByteSize());
	//delete ctx;
}

void SocketConnection::handleSystemInfo(const piscan::server::context::SystemInfo info){
	auto ctx = new piscan_pb::SystemInfo();

	ctx->set_version(info.version);
	ctx->set_build(info.buildNumber);
	ctx->set_squelchscalemin(info.squelchRange.first);
	ctx->set_squelchscalemax(info.squelchRange.second);
	*ctx->mutable_supportedmodulations() = {info.supportedModulations.begin(), info.supportedModulations.end()};

	piscan_pb::ServerToClient msg;
	msg.set_type(piscan_pb::ServerToClient_Type_REQUEST_RESPONSE);
	msg.set_allocated_systeminfo(ctx);

	msg.SerializeToArray(_writeBuffer, WRITE_BUFFER_LENGTH);
	_startWrite(_writeBuffer, msg.ByteSize());
	//delete ctx;
}

void SocketConnection::handleSignalLevel(const int level){
	//TODO
	(void) level;
	auto ctx = new piscan_pb::SignalLevel();
	ctx->set_level(level);

	piscan_pb::ServerToClient msg;
	msg.set_type(piscan_pb::ServerToClient_Type_SIGNAL_LEVEL);
	msg.set_allocated_signallevel(ctx);

	msg.SerializeToArray(_writeBuffer, WRITE_BUFFER_LENGTH);
	_startWrite(_writeBuffer, msg.ByteSize());
	//delete ctx;
}

void SocketConnection::_startRead() {
	_socket.async_read_some(boost::asio::buffer(_readBuffer, READ_BUFFER_LENGTH),
			boost::bind(&SocketConnection::_handleRead, shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

void SocketConnection::_startWrite(uint8_t* buffer, size_t length) {
	_socket.async_write_some(boost::asio::buffer(buffer, length),
			boost::bind(&SocketConnection::_handleWrite, shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}

void SocketConnection::_handleRead(const boost::system::error_code& err,
		size_t bytes_transferred) {
	if (!err) {
		// do stuff to read data
		piscan_pb::ClientToServer msg;
		msg.ParseFromArray(_readBuffer, bytes_transferred);

		switch(msg.type()){
		case piscan_pb::ClientToServer_Type_GENERAL_REQUEST:
			_handleGeneralRequest(msg.generalrequest());
			break;
		case piscan_pb::ClientToServer_Type_SCANNER_STATE_REQUEST:
			_handleScanStateRequest(msg.scanstaterequest());
			break;
		case piscan_pb::ClientToServer_Type_DEMOD_REQUEST:
			_handleDemodRequest(msg.demodrequest());
			break;
		default:
			LOG_F(WARNING, "Invalid ClientToServer message from %s", _socket.local_endpoint().address().to_string().c_str());
			break;
		}

		_startRead();
	} else if (err == boost::asio::error::eof || err == boost::asio::error::connection_reset || err == boost::asio::error::broken_pipe){
		disconnect();
	} else {
		LOG_F(WARNING, "SocketConnection error: %s", err.message().c_str());
		disconnect();
	}
}

void SocketConnection::_handleWrite(const boost::system::error_code& err,
		size_t /* bytes_transferred */) {
	if (!err) {

	} else if (err == boost::asio::error::eof || err == boost::asio::error::connection_reset || err == boost::asio::error::broken_pipe){
		disconnect();
	} else {
		LOG_F(WARNING, "SocketConnection error: %s", err.message().c_str());
		disconnect();
	}
}

void SocketConnection::_handleGeneralRequest(const piscan_pb::GeneralRequest& rq) {
//	static std::map<piscan_pb::GeneralRequest_RequestType, std::function<int(void)>> rqHandlers = {
//			{piscan_pb::GeneralRequest_RequestType_SCANNER_CONTEXT, [this]{return getScannerContext();}},
//			{piscan_pb::GeneralRequest_RequestType_DEMOD_CONTEXT, [this]{return getDemodContext();}},
//			{piscan_pb::GeneralRequest_RequestType_SYSTEM_INFO, [this]{return getSystemInfo();}},
//	};

	switch(rq.type()){
	case piscan_pb::GeneralRequest_RequestType_SCANNER_CONTEXT:
		getScannerContext();
		break;
	case piscan_pb::GeneralRequest_RequestType_DEMOD_CONTEXT:
		getDemodContext();
		break;
	case piscan_pb::GeneralRequest_RequestType_SYSTEM_INFO:
		getSystemInfo();
		break;
	default:
		LOG_F(WARNING, "Invalid GeneralRequest from %s", _socket.remote_endpoint().address().to_string().c_str());
		break;
	}

//	rqHandlers[rq.type()]();
}

void SocketConnection::_handleScanStateRequest(
		const piscan_pb::ScannerStateRequest& rq) {

	switch(rq.state()){
	case piscan_pb::ScannerStateRequest_NewState_SCAN:
		scanStart();
		break;
	case piscan_pb::ScannerStateRequest_NewState_HOLD:
		scanHold();
		break;
	case piscan_pb::ScannerStateRequest_NewState_MANUAL:
		scanManualEntry(rq.manfreq());
		break;
	default:
		LOG_F(WARNING, "Invalid ScannerStateRequest from %s", _socket.local_endpoint().address().to_string().c_str());
		break;
	}
}

void SocketConnection::_handleDemodRequest(const piscan_pb::DemodRequest& rq) {
	switch(rq.type()){
	case piscan_pb::DemodRequest_DemodFunc_SET_SQUELCH:
		setDemodSquelch(rq.level());
		break;
	case piscan_pb::DemodRequest_DemodFunc_SET_GAIN:
		setDemodGain(rq.level());
		break;
	default:
		LOG_F(WARNING, "Invalid DemodRequest");
		break;
	}
}

}
}
}
