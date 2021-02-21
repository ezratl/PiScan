/*
 * DbusServer.h
 *
 *  Created on: Oct 22, 2018
 *      Author: ezra
 */

#ifndef SERVER_SOCKETSERVER_H_
#define SERVER_SOCKETSERVER_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <unistd.h>

#include "connection/SocketConnection.h"
#include "BackendServer.h"
#include "ServerManager.h"
#include "request.pb.h"
#include "constants.h"
#include "Configuration.h"

using namespace boost::asio;
using ip::tcp;

namespace piscan {
namespace server {

namespace connection {
typedef boost::shared_ptr<SocketConnection> SocketConnectionPtr;
}

/* listener for socket connections */
class SocketServer: public BackendServer {
public:
	SocketServer(ServerInterface& host, boost::asio::io_service& io_service) : BackendServer(host), _ioService(io_service), _acceptor(io_service) {};
	~SocketServer() {};

	void start();
	void stop();
	void spawnLocalClient();

private:
	io_service& _ioService;
	tcp::acceptor _acceptor;
	uint16_t _listenPort = DEFAULT_TCP_PORT;
	// int _activeConnections = 0;
	pid_t _clientPid = 0;

	void start_accept();
	void handle_accept(connection::SocketConnectionPtr connection,
	                     const boost::system::error_code& err);

	void _spawnPythonClient();
	void _stopPythonClient();
};
}
}
#endif /* SERVER_SOCKETSERVER_H_ */
