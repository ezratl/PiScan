#include <condition_variable>
#include <csignal>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <boost/asio.hpp>
//using namespace std;

#include "PiScan.h"
#include "constants.h"
#include "Configuration.h"
#include "sigproc_types.h"
#include "Demodulator.h"
#include "Entry.h"
#include "loguru.hpp"
#include "messages.h"
#include "ServerManager.h"
#include "ScannerSM.h"
#include "SystemList.h"
#include "threadname.h"

#include "version.h"
#ifndef PISCAN_VERSION
#define PISCAN_VERSION	"debug"
#warning "PiScan version not defined in CMake"
#endif

//enum {
//	SYSTEM_CONTROL,
//	SCANNER_SM,
//	DEMOD,
//	SERVER_MAN,
//	AUDIO_MAN,
//	CLIENT,
//};

#define IO_THREAD_NAME	"IO Service"

#define SCANNER_FLAG	0x01
#define CONNMGR_FLAG	0x02
#define DEMOD_FLAG		0x04
#define AUDIO_FLAG		0x08

//#define ALL_FLAG		0x0F
#define ALL_FLAG		0x07

//using namespace piscan;
namespace piscan {

static bool sysRun;

static boost::asio::io_service io_service;
static std::thread ioThread;
static piscan::scan::SystemList scanSystems;
static ScannerSM scannerInst(scanSystems);
static ServerManager connectionManager(io_service);
static piscan::sigproc::Demodulator demodInst;

static std::atomic_bool steadyState(false);

void hardTerminate(){
	LOG_F(WARNING, "Terminating - resources may not be properly released");
	std::terminate();
}

void sigTermHandler(int /* signal */){
	// SIGTERM is raised by the kernel during shutdown
	if(sysRun){
		app::system::stopSystem();
		sysRun = false;
	}
	// SIGTERM called after exit process has started - possibly because the program locked up and the user wants to force quit
	else
		piscan::hardTerminate();
}

void sigIntHandler(int /* signal */){
	LOG_F(INFO, "Stop triggered by interrupt");

	if(!sysRun)
		piscan::hardTerminate();

	sysRun = false;
}

piscan::sigproc::DemodInterface& app::demod::getDemodInstance() {
	return demodInst;
}

void runIO(){
	setThreadName(IO_THREAD_NAME);
	DLOG_F(2, "Starting IO service");
	io_service.run();
}

void exit(int code){
	LOG_F(INFO, "PiScan stopped, exiting");

	io_service.stop();
	ioThread.join();

	
	std::exit(code);
}

bool app::system::stopSystem(){
	if(steadyState.load()){
		return true;
	}
	return false;
}

void app::scanner::startScan(){
	scannerInst.startScan();
}

void app::scanner::holdScan(std::vector<int> index){
	scannerInst.holdScan(index);
}

void app::scanner::stopScanner(){
	scannerInst.stopScanner();
}

void app::scanner::manualEntry(app::ManualEntryData* freq){
	scannerInst.manualEntry(freq);
}

piscan::server::context::ScannerContext app::scanner::getScannerContext(){
	return scannerInst.getCurrentContext();
}

void app::demod::setTunerGain(float gain){
	demodInst.setTunerGain(gain);
}

void app::demod::setDemodSquelch(float level){
	demodInst.setSquelch(level);
}

piscan::server::context::DemodContext app::demod::getDemodContext(){
	return piscan::server::context::DemodContext(demodInst.getTunerGain(), demodInst.getSquelch());
}

void app::demod::squelchBreak(bool mute){
	demodInst.squelchBreak(mute);
}

long long app::demod::getTunerSampleRate() {
	return demodInst.getTunerSampleRate();
}

const piscan::server::context::SystemInfo app::system::getSystemInfo(){
	piscan::server::context::SystemInfo info = {
			.version = PISCAN_VERSION,
			.buildNumber = 0,
			.squelchRange = {0, 0},
			.supportedModulations = {"FM", "AM"},
	};
	switch(getConfig().getDemodConfig().squelchType){
	case SQUELCH_PCT:
	case SQUELCH_SNR:
		info.squelchRange = {0, 100};
		break;
	case SQUELCH_DBM:
	default:
		info.squelchRange = {-100, 0};
	}
	return info;
}

void app::server::scannerContextUpdate(piscan::server::context::ScannerContext ctx){
	connectionManager.giveMessage(make_shared<ServerMessage>(SCANNER_SM, ServerMessage::CONTEXT_UPDATE, new piscan::server::context::ScannerContext(ctx)));
}

void app::server::demodContextUpdate(piscan::server::context::DemodContext ctx){
	connectionManager.giveMessage(make_shared<ServerMessage>(DEMOD, ServerMessage::CONTEXT_UPDATE, new piscan::server::context::DemodContext(ctx)));
}

void app::server::signalLevelUpdate(int level){
	connectionManager.giveMessage(make_shared<ServerMessage>(DEMOD, ServerMessage::SIGNAL_LEVEL, new int(level)));
}

}

using namespace piscan;

int main(int argc, char **argv) {
	loguru::init(argc, argv);

	loguru::g_preamble_file = false;

	signal(SIGINT, sigIntHandler);
	signal(SIGTERM, sigTermHandler);

	LOG_F(INFO, "Starting PiScan, version %s", PISCAN_VERSION);

	piscan::config::Configuration& config = piscan::config::Configuration::getConfig();
	bool useDebugConsole = false;
	bool spawnClient = false;

	int logVerbosity = config.getGeneralConfig().logfileVerbosity;

	int c;
	while((c = getopt(argc,argv,"dp:f:l")) != -1){
		switch(c){
			case 'd':
				useDebugConsole = true;
				break;
			case 'p':
				if(optarg)
					config.setWorkingDirectory(std::string(optarg));
				break;
			case 'f':
				if(optarg)
					logVerbosity = std::atoi(optarg);
				break;
			case 'l':
				spawnClient = true;
				break;
		}
	}

	
	config.loadConfig();
	config.loadState();

	loguru::add_file(config.getDatedLogPath().c_str(), loguru::Truncate, logVerbosity);
	loguru::add_file(config.getLatestLogPath().c_str(), loguru::Truncate, logVerbosity);

	try {
		{
			scannerInst.start();
			connectionManager.start(useDebugConsole, spawnClient);
			demodInst.start();

			LOG_F(4, "scanner wait");
			scannerInst.waitReady();
			LOG_F(4, "server wait");
			connectionManager.waitReady();
			LOG_F(4, "demod wait");
			demodInst.waitReady();

			connectionManager.allowConnections();
			scannerInst.startScanner();

			sysRun = true;
		}

		ioThread = std::thread(runIO);
	} catch (std::exception& e) {
		LOG_F(ERROR, "%s", e.what());
		goto sysexit;
	}

	steadyState.store(true);
	LOG_F(INFO, "System initialized");

	while(sysRun)
		usleep(100000);

	sysexit:
	steadyState.store(false);
	try {
		//sysControl.stop();

		{
			LOG_F(INFO, "Stopping system");
		scannerInst.stopScanner();
		connectionManager.stop();
		demodInst.stop();

		LOG_F(4, "scanner wait");
		scannerInst.waitDeinit();
		LOG_F(4, "server wait");
		connectionManager.waitDeinit();
		LOG_F(4, "demod wait");
		demodInst.waitDeinit();

		LOG_F(2, "All modules stopped");
		}
	} catch (std::exception& e) {
		LOG_F(ERROR, "%s", e.what());
	}

	config.saveState();
	config.saveConfig();

	piscan::exit(0);

	return 0;
}

