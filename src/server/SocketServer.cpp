/*
 * DbusServer.cpp
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <sys/wait.h>
#include <errno.h>

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

void SocketConnection::contextUpdate(const ScannerContext context){
	auto ctx = new piscan_pb::ScannerContext();
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
	delete ctx;
}

void SocketConnection::contextUpdate(const DemodContext context){
	piscan_pb::DemodContext* ctx = new piscan_pb::DemodContext();

	ctx->set_gain(context.gain);
	ctx->set_squelch(context.squelch);

	piscan_pb::ServerToClient msg;
	msg.set_type(piscan_pb::ServerToClient_Type_DEMOD_CONTEXT);
	msg.set_allocated_demodcontext(ctx);

	msg.SerializeToArray(_writeBuffer, WRITE_BUFFER_LENGTH);
	_startWrite(_writeBuffer, msg.ByteSize());
	delete ctx;
}

void SocketConnection::handleSystemMessage(const GeneralMessage message) {
	auto ctx = new piscan_pb::GeneralMessage();

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
	delete ctx;
}

void SocketConnection::handleSystemInfo(const SystemInfo info){
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
	delete ctx;
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
	delete ctx;
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
	static std::map<piscan_pb::GeneralRequest_RequestType, std::function<int(void)>> rqHandlers = {
			{piscan_pb::GeneralRequest_RequestType_SCANNER_CONTEXT, [this]{return getScannerContext();}},
			{piscan_pb::GeneralRequest_RequestType_DEMOD_CONTEXT, [this]{return getDemodContext();}},
			{piscan_pb::GeneralRequest_RequestType_SYSTEM_INFO, [this]{return getSystemInfo();}},
	};

	/*switch(rq.type()){
	case piscan_pb::GeneralRequest_RequestType_SCANNER_CONTEXT:
		getScannerContext();
		break;
	case piscan_pb::GeneralRequest_RequestType_DEMOD_CONTEXT:
		getDemodContext();
		break;
	default:
		LOG_F(WARNING, "Invalid GeneralRequest from %s", _socket.remote_endpoint().address().to_string().c_str());
		break;
	}*/

	rqHandlers[rq.type()]();
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


/*****************************************/

void SocketServer::start() {
	SocketServerConfig& config = Configuration::getConfig().getSocketConfig();
	_listenPort = config.tcpPort;

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

	if(config.spawnLocalClient)
		_spawnPythonClient();
}

void SocketServer::stop(){
	if (_acceptor.is_open()) {
		LOG_F(INFO, "Stopping TCP server");
		_acceptor.close();
	}

	//if(_clientPid > 0)
	//	_stopPythonClient();
}

void SocketServer::spawnLocalClient(){
	SocketServerConfig& config = Configuration::getConfig().getSocketConfig();
	if(!config.spawnLocalClient)
		_spawnPythonClient();
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

void SocketServer::_spawnPythonClient(){
	LOG_F(INFO, "Creating local GUI client");

	SocketServerConfig& config = Configuration::getConfig().getSocketConfig();
	std::string scriptPath = config.pythonClient;
	std::string pythonPath = config.pythonBinary;

	const char* argv[16];
	argv[0] = pythonPath.c_str();
	argv[1] = "client.py";
	argv[2] = "-l";
	argv[3] = "-p";
	argv[4] = std::to_string(config.tcpPort).c_str();
	argv[5] = NULL;

	_clientPid = fork();

	if(_clientPid == 0){
		//child process
		LOG_F(1, "Python command: %s", argv[0]);
		LOG_F(1, "Client path: %s", argv[1]);

		chdir(scriptPath.c_str());

		execvp(argv[0], const_cast<char* const*>(argv));

		LOG_F(ERROR, "Starting client failed: %s", strerror(errno));
		exit(0);
	}
	else if(_clientPid > 0){
		// parent
		LOG_F(1, "Process creation success");
		signal(SIGCHLD, SIG_IGN);
	}
	else {
		// fork failed
		LOG_F(ERROR, "Failed to create local client");
	}
}

void SocketServer::_stopPythonClient(){
	// send SIGINT to client
	int status;
	pid_t childPid = waitpid(_clientPid, &status, 0);

	if(childPid == -1)
		LOG_F(ERROR, "waitpid failed");
}
