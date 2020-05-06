/*
 * AudioStreamServer.h
 *
 *  Created on: May 3, 2020
 *      Author: ezra
 */

#pragma once

#include <BackendServer.h>

#include "liveMedia.hh"

namespace piscan {

class LocalPCMSource;

class AudioStreamServer: public BackendServer {
public:
	AudioStreamServer(ServerInterface& host);
	~AudioStreamServer();

	void start();
	void stop();

	LocalPCMSource* startFromAudioController(int sampleRate);

	void giveMessage(std::shared_ptr<Message> message) { (void)message; };

private:
	std::thread _serverThread;

	RTSPServer* _rtspServer;
};

} /* namespace piscan */
