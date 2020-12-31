#pragma once

#include <boost/shared_ptr.hpp>

namespace piscan::server::connection {
	class Connection;
	typedef boost::shared_ptr<Connection> ConnectionPtr;

	class DebugConsole;
	class SocketConnection;
}
