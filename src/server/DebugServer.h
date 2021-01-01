/*
 * ServerDebugOutput.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef SERVERDEBUGOUTPUT_H_
#define SERVERDEBUGOUTPUT_H_

#include <thread>
#include <memory>
#include <boost/shared_ptr.hpp>

#include "server_types.h"
#include "connection/connection_types.h"
#include "BackendServer.h"
#include "messages.h"

namespace piscan {
namespace server {

class DebugServer : public BackendServer {
public:
	DebugServer(ServerInterface& host);
	~DebugServer() {};

	void start();
	void stop();
	
private:
	boost::shared_ptr<connection::DebugConsole> _connection;
};

}
}
#endif /* SERVERDEBUGOUTPUT_H_ */
