/*
 * AudioStreamServer.h
 *
 *  Created on: May 3, 2020
 *      Author: ezra
 */

#pragma once

#include "server_types.h"
#include "BackendServer.h"
#include "liveMedia.hh"
#include "messages.h"

namespace piscan::server {

namespace audio {
class LocalPCMSource;
}

class AudioStreamServer: public BackendServer {
public:
	AudioStreamServer(ServerInterface& host);
	~AudioStreamServer();

	void start();
	void stop();

	audio::LocalPCMSource* startFromAudioController(int sampleRate);

	void giveMessage(std::shared_ptr<Message> message) { (void)message; };

private:
	std::thread _serverThread;

	RTSPServer* _rtspServer;
	char volatile _eventLoopWatch;
};

} /* namespace piscan */
