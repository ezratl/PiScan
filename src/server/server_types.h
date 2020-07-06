#pragma once

#include <boost/shared_ptr.hpp>

#include "connection/connection_types.h"

namespace piscan::server {
	class ServerInterface {
	public:
		virtual ~ServerInterface() {};

		enum RequestResponse {
			RQ_ACCEPTED,
			RQ_DENIED,
			RQ_INSUFFICIENT_PERMISSION,
			RQ_INVALID_HANDLE,
		};

		virtual int requestConnection(connection::ConnectionPtr client) = 0;
		virtual int giveRequest(void* request) = 0;
	};

	class BackendServer;

	class DebugServer;
	class SocketServer;
	class AudioStreamServer;
}
