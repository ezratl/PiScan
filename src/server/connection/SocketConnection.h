/*
 * SocketConnection.h
 *
 *  Created on: Jul 5, 2020
 *      Author: ezra
 */

#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <unistd.h>

#include "BackendServer.h"
#include "ServerManager.h"
#include "request.pb.h"
#include "connection.h"
#include "constants.h"

using namespace boost::asio;
using ip::tcp;

#define READ_BUFFER_LENGTH	1024
#define WRITE_BUFFER_LENGTH	1024

namespace piscan::server::connection {

typedef boost::shared_ptr<SocketConnection> SocketConnectionPtr;

class SocketConnection : public Connection, public boost::enable_shared_from_this<SocketConnection> {
public:
	SocketConnection(boost::asio::io_service& io_service, ConnectionLevel lvl) : Connection(lvl), _socket(io_service) {};
	~SocketConnection() {};

	void giveMessage(std::shared_ptr<Message> message);
	bool connect();
	void disconnect();
	void contextUpdate(const piscan::server::context::ScannerContext context);
	void contextUpdate(const piscan::server::context::DemodContext context);
	void handleSystemMessage(const piscan::server::context::GeneralMessage message);
	void handleSystemInfo(const piscan::server::context::SystemInfo info);
	void handleSignalLevel(const int level);

	tcp::socket& socket() { return _socket; };

	static SocketConnectionPtr create(boost::asio::io_service& io_service) {
		return SocketConnectionPtr(new SocketConnection(io_service, FULL_CONTROL));
	}

	const std::string identifier() {
		return _socket.remote_endpoint().address().to_string();
	}
private:
	tcp::socket _socket;
	unsigned char _readBuffer[READ_BUFFER_LENGTH];
	unsigned char _writeBuffer[WRITE_BUFFER_LENGTH];

	void _startRead();
	void _startWrite(uint8_t* buffer, size_t length);

	void _handleRead(const boost::system::error_code& err,
			size_t bytes_transferred);
	void _handleWrite(const boost::system::error_code& err,
			size_t bytes_transferred);

	void _handleGeneralRequest(const piscan_pb::GeneralRequest& rq);
	void _handleScanStateRequest(const piscan_pb::ScannerStateRequest& rq);
	void _handleDemodRequest(const piscan_pb::DemodRequest& rq);

};

}
