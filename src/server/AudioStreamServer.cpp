/*
 * AudioStreamServer.cpp
 *
 *  Created on: May 3, 2020
 *      Author: ezra
 *
 *  Based on 'testOnDemandRTSPServer.cpp' from the live555 library
 */

#include <AudioStreamServer.h>

#include "PiScan.h"
#include "loguru.hpp"
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "threadname.h"
#include "AudioServerMediaSubsession.h"

namespace piscan {

AudioStreamServer::AudioStreamServer(ServerInterface& host) : BackendServer(host) {
	AudioServerConfig& config = app::getConfig().getAudioServerConfig();


	// Begin by setting up our usage environment:
	    TaskScheduler *scheduler = BasicTaskScheduler::createNew();
	    UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);

	    UserAuthenticationDatabase *authDB = NULL;
	#ifdef ACCESS_CONTROL
	    // To implement client access control to the RTSP server, do the following:
	    authDB = new UserAuthenticationDatabase;
	    authDB->addUserRecord("username1", "password1"); // replace these with real strings
	                                                     // Repeat the above with each <username>, <password> that you wish to allow
	                                                     // access to the server.
	#endif

	    // Create the RTSP server:
	    _rtspServer = RTSPServer::createNew(*env, config.rtspPort, authDB);
	    if (_rtspServer == NULL)
	    {
	        //*env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
	        //exit(1);
	    	LOG_F(ERROR, "Failed to create RTSP server: %s", env->getResultMsg());
	    }


}

AudioStreamServer::~AudioStreamServer() {
	// TODO Auto-generated destructor stub
}

void AudioStreamServer::start(){
	if(!_rtspServer)
		return;

	LOG_F(1, "Starting audio RTSP server");

	char const *descriptionString = "PiScan audio stream";
	Boolean reuseFirstSource = True; // only one stream object will exist at a time regardless of clients listening

	char const *streamName = "audio";
	ServerMediaSession *sms = ServerMediaSession::createNew(
			_rtspServer->envir(), streamName, streamName, descriptionString);
	// To convert 16-bit PCM data to 8-bit u-law, prior to streaming,
	// change the following to True:
	Boolean convertToULaw = true;
	AudioServerMediaSubsession* ses = AudioServerMediaSubsession::createNew(
			_rtspServer->envir(), reuseFirstSource, convertToULaw);
	sms->addSubsession(ses);
	_rtspServer->addServerMediaSession(sms);


	LOG_F(INFO, "RTSP audio server available on port %i",
			app::getConfig().getAudioServerConfig().rtspPort);
	LOG_F(1, "URL for stream clients: %s", _rtspServer->rtspURL(sms));

	if (app::getConfig().getAudioServerConfig().httpTunneling) {
		// Also, attempt to create a HTTP server for RTSP-over-HTTP tunneling.
		// Try first with the default HTTP port (80), and then with the alternative HTTP
		// port numbers (8000 and 8080).
		if (_rtspServer->setUpTunnelingOverHTTP(80)
				|| _rtspServer->setUpTunnelingOverHTTP(8000)
				|| _rtspServer->setUpTunnelingOverHTTP(8080)) {
			LOG_F(INFO, "RTSP-over-HTTP tunneling available on port %i", _rtspServer->httpServerPortNum());
		} else {
			LOG_F(WARNING, "RTSP-over-HTTP tunneling is not available");
		}
	}


	// TODO currently no way to terminate this thread in a friendly manner
	_serverThread = std::thread([this]{
		setThreadName("RTSP Server");
		_rtspServer->envir().taskScheduler().doEventLoop(); // does not return
	});
}

void AudioStreamServer::stop() {

}

} /* namespace piscan */
