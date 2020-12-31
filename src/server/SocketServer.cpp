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
#include "connection/SocketConnection.h"
#include "context.pb.h"
#include "messages.pb.h"

namespace piscan::server {

void SocketServer::start() {
	piscan::config::SocketServerConfig& config = piscan::config::Configuration::getConfig().getSocketConfig();
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
	piscan::config::SocketServerConfig& config = piscan::config::Configuration::getConfig().getSocketConfig();
	if(!config.spawnLocalClient)
		_spawnPythonClient();
}

void SocketServer::giveMessage(std::shared_ptr<Message> message){

}

void SocketServer::start_accept() {
	// creates a socket
	connection::SocketConnectionPtr connection = connection::SocketConnection::create(_ioService);

	// initiates an asynchronous accept operation
	// to wait for a new connection.
	_acceptor.async_accept(connection->socket(),
			boost::bind(&SocketServer::handle_accept, this, connection,
					boost::asio::placeholders::error));
}

void SocketServer::handle_accept(connection::SocketConnectionPtr connection,
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

	piscan::config::SocketServerConfig& config = piscan::config::Configuration::getConfig().getSocketConfig();
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

}
