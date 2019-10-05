/*
 * DbusServer.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

#include <boost/asio.hpp>
#include <boost/asio/error.hpp>

#include "SocketServer.h"
#include "loguru.hpp"

#include "context.pb.h"
#include "messages.pb.h"

using namespace piscan;

void SocketConnection::giveMessage(std::shared_ptr<Message> message){

}

bool SocketConnection::connect(){
	_startRead();
	return true;
}

void SocketConnection::disconnect(){
	if (_socket.is_open()) {
		_socket.close();
		notifyDisconnected();
	}
}

void SocketConnection::contextUpdate(ScannerContext context){
	piscan_pb::ScannerContext* ctx = new piscan_pb::ScannerContext();
	switch(context.state){
	case ScannerContext::ScannerState::OTHER_STATE:
		ctx->set_state(piscan_pb::ScannerContext_State_INVAL);
		break;
	case ScannerContext::ScannerState::SCAN:
		ctx->set_state(piscan_pb::ScannerContext_State_SCAN);
		break;
	case ScannerContext::ScannerState::HOLD:
		ctx->set_state(piscan_pb::ScannerContext_State_HOLD);
		break;
	case ScannerContext::ScannerState::RECEIVE:
		ctx->set_state(piscan_pb::ScannerContext_State_RECEIVE);
		break;
	default:
		ctx->set_state(piscan_pb::ScannerContext_State_INVAL);
	}

	ctx->set_freq(context.frequency);
	ctx->set_systemtag(context.systemTag);
	ctx->set_entrytag(context.entryTag);
	ctx->set_modulation(context.modulation);
	ctx->set_entryindex(context.entryIndex.c_str());

	piscan_pb::ServerToClient msg;
	msg.set_type(piscan_pb::ServerToClient_Type_SCANNER_CONTEXT);
	msg.set_allocated_scannercontext(ctx);

	msg.SerializeToArray(_writeBuffer, WRITE_BUFFER_LENGTH);
	_startWrite(_writeBuffer, msg.ByteSize());
}

void SocketConnection::contextUpdate(DemodContext context){
	piscan_pb::DemodContext* ctx = new piscan_pb::DemodContext();

	ctx->set_gain(context.gain);
	ctx->set_squelch(context.squelch);

	piscan_pb::ServerToClient msg;
	msg.set_type(piscan_pb::ServerToClient_Type_DEMOD_CONTEXT);
	msg.set_allocated_demodcontext(ctx);

	msg.SerializeToArray(_writeBuffer, WRITE_BUFFER_LENGTH);
	_startWrite(_writeBuffer, msg.ByteSize());
}

void SocketConnection::systemMessage(GeneralMessage message) {
	piscan_pb::GeneralMessage* ctx = new piscan_pb::GeneralMessage();

	switch(message.type){
	case GeneralMessage::INFO:
		ctx->set_type(piscan_pb::GeneralMessage_Type_INFO);
		break;
	case GeneralMessage::WARNING:
		ctx->set_type(piscan_pb::GeneralMessage_Type_WARNING);
		break;
	case GeneralMessage::ERROR:
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
		size_t bytes_transferred) {
	if (!err) {

	} else if (err == boost::asio::error::eof || err == boost::asio::error::connection_reset || err == boost::asio::error::broken_pipe){
		disconnect();
	} else {
		LOG_F(WARNING, "SocketConnection error: %s", err.message().c_str());
		disconnect();
	}
}

void SocketConnection::_handleGeneralRequest(const piscan_pb::GeneralRequest& rq) {
	switch(rq.type()){
	case piscan_pb::GeneralRequest_RequestType_SCANNER_CONTEXT:
		getScannerContext();
		break;
	case piscan_pb::GeneralRequest_RequestType_DEMOD_CONTEXT:
		getDemodContext();
		break;
	default:
		LOG_F(WARNING, "Invalid GeneralRequest from %s", _socket.local_endpoint().address().to_string().c_str());
		break;
	}
}

void SocketConnection::_handleScanStateRequest(
		const piscan_pb::ScannerStateRequest& rq) {
	//ScannerFunction func;
	uint32_t freq = 0;

	switch(rq.state()){
	case piscan_pb::ScannerStateRequest_NewState_SCAN:
		//func = SCAN;
		scanStart();
		break;
	case piscan_pb::ScannerStateRequest_NewState_HOLD:
		//func = HOLD;
		scanHold();
		break;
	case piscan_pb::ScannerStateRequest_NewState_MANUAL:
		//func = MANUAL;
		//freq = static_cast<uint32_t>(rq.manfreq());
		scanManualEntry(rq.manfreq());
		break;
	default:
		LOG_F(WARNING, "Invalid ScannerStateRequest from %s", _socket.local_endpoint().address().to_string().c_str());
		break;
	}

	//scannerFunction(func, freq);
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


/*****************************************/

void SocketServer::start() {
	LOG_F(INFO, "Starting TCP server on port %i", _listenPort);

	// exception thrown when binding TCP port fails
	try {
		tcp::endpoint ep(tcp::v4(), _listenPort);
		_acceptor.open(ep.protocol());
		_acceptor.bind(ep);
		_acceptor.listen();

		start_accept();
	} catch (std::exception& e) {
		LOG_F(ERROR, "Exception caught: %s", e.what());
		stop();
	}
}

void SocketServer::stop(){
	if (_acceptor.is_open()) {
		LOG_F(INFO, "Stopping TCP server");
		_acceptor.close();
	}
}

void SocketServer::giveMessage(std::shared_ptr<Message> message){

}

void SocketServer::start_accept() {
	// creates a socket
	SocketConnection::pointer connection = SocketConnection::create(_acceptor.get_io_service());

	// initiates an asynchronous accept operation
	// to wait for a new connection.
	_acceptor.async_accept(connection->socket(),
			boost::bind(&SocketServer::handle_accept, this, connection,
					boost::asio::placeholders::error));
}

void SocketServer::handle_accept(SocketConnection::pointer connection,
		const boost::system::error_code& err){

	if (!err) {
		//connection->start();
		LOG_F(1, "TCP connection initiated");
		int ec = _host.requestConnection(connection);
		if(ec != 0){
			LOG_F(1, "TCP connection failed, code %i", ec);
		}
	}
	start_accept();
}

