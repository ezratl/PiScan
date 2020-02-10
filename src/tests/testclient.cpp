#include <iostream>
#include <unistd.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "connection.h"
#include "clientmessage.h"
#include "DebugServer.h"
#include "loguru.hpp"

#include "messages.pb.h"
#include "context.pb.h"

#define READ_BUFFER_LENGTH	1024
#define WRITE_BUFFER_LENGTH	1024

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

bool sysRun = false;

namespace piscan {

class TestClient : public ServerInterface, public boost::enable_shared_from_this<TestClient>{
public:
	TestClient(io_service& io_service, tcp::socket& socket) : _console(*this), _io_service(io_service), _socket(socket) {
		//_console(this);

	}

	~TestClient() {}

	void run() {
		sysRun = true;
		_console.start();
		_startRead();
	}

	static boost::shared_ptr<TestClient> create(io_service& io_service, tcp::socket& socket){
		return boost::shared_ptr<TestClient>(new TestClient(io_service, socket));
	}

private:
	DebugServer _console;

	io_service& _io_service;
	tcp::socket& _socket;
	unsigned char _readBuffer[READ_BUFFER_LENGTH];
	unsigned char _writeBuffer[WRITE_BUFFER_LENGTH];

	boost::shared_ptr<Connection> _conn;

	void _startRead() {
		_socket.async_read_some(boost::asio::buffer(_readBuffer, READ_BUFFER_LENGTH),
				boost::bind(&TestClient::_handleRead, shared_from_this(),
						placeholders::error,
						placeholders::bytes_transferred));
	}

	void _startWrite(uint8_t* buffer, size_t length) {
		_socket.async_write_some(boost::asio::buffer(buffer, length),
				boost::bind(&TestClient::_handleWrite, shared_from_this(),
						placeholders::error,
						placeholders::bytes_transferred));
	}

	void _handleRead(const boost::system::error_code& err,
			size_t bytes_transferred) {
		if (!err) {
			// do stuff to read data
			cout << "received:" << endl;

			piscan_pb::ServerToClient msg;
			msg.ParseFromArray(_readBuffer, bytes_transferred);

			cout << msg.DebugString() << endl;

			_startRead();
		} else {
			cout << "SocketConnection error: " << err.message() << endl;
			_disconnect();
		}
	}

	void _handleWrite(const boost::system::error_code& err,
			size_t bytes_transferred) {
		if (!err) {

		} else {
			cout << "SocketConnection error: " << err.message() << endl;
			_disconnect();
		}
	}

	int requestConnection(boost::shared_ptr<Connection> client) {
		_conn = client;
		client.get()->_serverManager = this;
		client.get()->connect();
		return 0;
	}

	int giveRequest(void* request) {
		ClientRequest* rq = reinterpret_cast<ClientRequest*>(request);

		piscan_pb::ClientToServer msg;
		piscan_pb::GeneralRequest* grq;
		piscan_pb::ScannerStateRequest* srq;
		piscan_pb::DemodRequest* drq;

		switch(rq->rqInfo.type){
		case NOTIFY_DISCONNECTED:
			sysRun = false;
			break;
		case SYSTEM_FUNCTION:
			if(rq->rqInfo.subType == SYSTEM_STOP){
				//_disconnect();
				sysRun = false;
			}
			break;
		case SCANNER_FUNCTION:
			if (rq->rqInfo.subType == SCANNER_GET_CONTEXT) {
				grq = new piscan_pb::GeneralRequest();
				grq->set_type(piscan_pb::GeneralRequest_RequestType_SCANNER_CONTEXT);
				msg.set_type(piscan_pb::ClientToServer_Type_GENERAL_REQUEST);
				msg.set_allocated_generalrequest(grq);
			} else {
				srq = new piscan_pb::ScannerStateRequest();

				switch (rq->rqInfo.subType) {
				case SCANNER_STATE_SCAN:
					srq->set_state(piscan_pb::ScannerStateRequest_NewState_SCAN);
					break;
				case SCANNER_STATE_HOLD:
					srq->set_state(piscan_pb::ScannerStateRequest_NewState_HOLD);
					break;
				case SCANNER_STATE_MANUAL:
					srq->set_state(piscan_pb::ScannerStateRequest_NewState_MANUAL);
					srq->set_manfreq(static_cast<unsigned long int>(*(reinterpret_cast<uint32_t*>(rq->pData))));
					delete (reinterpret_cast<uint32_t*>(rq->pData));
					break;
				default:
					break;
				}

				msg.set_type(piscan_pb::ClientToServer_Type_SCANNER_STATE_REQUEST);
				msg.set_allocated_scanstaterequest(srq);
			}
			break;
		case DATABASE_RETRIEVE:
			break;
		case DATABASE_MODIFY:
			break;
		case CONFIG_RETRIEVE:
			break;
		case CONFIG_MODIFY:
			break;
		case DEMOD_CONFIGURE:
			drq = new piscan_pb::DemodRequest();

			switch(rq->rqInfo.subType){
			case DEMOD_SET_GAIN:
				drq->set_type(piscan_pb::DemodRequest_DemodFunc_SET_GAIN);
				drq->set_level(*(reinterpret_cast<int*>(rq->pData)));
				delete (reinterpret_cast<int*>(rq->pData));
				break;
			case DEMOD_SET_SQUELCH:
				drq->set_type(piscan_pb::DemodRequest_DemodFunc_SET_SQUELCH);
				drq->set_level(*(reinterpret_cast<int*>(rq->pData)));
				delete (reinterpret_cast<int*>(rq->pData));
				break;
			default:
				break;
			}

			msg.set_type(piscan_pb::ClientToServer_Type_DEMOD_REQUEST);
			msg.set_allocated_demodrequest(drq);
			break;
		case GET_CONTEXT:
			grq = new piscan_pb::GeneralRequest();

			switch(rq->rqInfo.subType){
			case SCANNER_CONTEXT:
				grq->set_type(piscan_pb::GeneralRequest_RequestType_SCANNER_CONTEXT);
				break;
			case DEMOD_CONTEXT:
				grq->set_type(piscan_pb::GeneralRequest_RequestType_DEMOD_CONTEXT);
				break;
			default:
				break;
			}

			msg.set_type(piscan_pb::ClientToServer_Type_GENERAL_REQUEST);
			msg.set_allocated_generalrequest(grq);
			break;
		default:
			break;
		}

		delete rq;

		cout << msg.DebugString() << endl;

		msg.SerializeToArray(_writeBuffer, WRITE_BUFFER_LENGTH);

		_startWrite(_writeBuffer, WRITE_BUFFER_LENGTH);

		return 0;
	}

public:
	void _disconnect() {
		_socket.close();
		//_io_service.stop();
		_conn.get()->disconnect();
	}
};

}

using namespace piscan;

int main(int argc, char **argv) {
	loguru::init(argc, argv);

	io_service io_service;
	tcp::socket socket(io_service);

	std::string address;
	cout << "Address: ";
	std::cin >> address;

	socket.connect(
			tcp::endpoint(ip::address::from_string(address),
					1234));

	boost::shared_ptr<TestClient> client = TestClient::create(io_service,
			socket);

	client->run();

	io_service.run();

	while (sysRun)
		usleep(100);

	client->_disconnect();

	io_service.stop();

	return 0;
}
