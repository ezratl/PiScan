/*
 * DbusServer.h
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

#ifndef SERVER_SOCKETSERVER_H_
#define SERVER_SOCKETSERVER_H_

#include "BackendServer.h"
#include "constants.h"
#include "ServerManager.h"
#include "request.pb.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
using ip::tcp;

#define READ_BUFFER_LENGTH	1024
#define WRITE_BUFFER_LENGTH	1024

namespace piscan {

class SocketConnection : public Connection, public boost::enable_shared_from_this<SocketConnection> {
public:
	typedef boost::shared_ptr<SocketConnection> pointer;

	SocketConnection(boost::asio::io_service& io_service, ConnectionLevel lvl) : Connection(lvl), _socket(io_service) {};
	~SocketConnection() {};

	void giveMessage(std::shared_ptr<Message> message);
	bool connect();
	void disconnect();
	void contextUpdate(ScannerContext context);
	void contextUpdate(DemodContext context);
	void systemMessage(GeneralMessage message);

	tcp::socket& socket() { return _socket; };

	static pointer create(boost::asio::io_service& io_service) {
		return pointer(new SocketConnection(io_service, FULL_CONTROL));
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

/* listener for socket connections */
class SocketServer: public BackendServer {
public:
	SocketServer(ServerInterface& host, boost::asio::io_service& io_service) : BackendServer(host), _acceptor(io_service) {};
	~SocketServer() {};

	void start();
	void stop();

	void giveMessage(std::shared_ptr<Message> message);
private:
	tcp::acceptor _acceptor;
	uint16_t _listenPort = DEFAULT_TCP_PORT;
	int _activeConnections = 0;

	void start_accept();
	void handle_accept(SocketConnection::pointer connection,
	                     const boost::system::error_code& err);
};
}
#endif /* SERVER_SOCKETSERVER_H_ */
