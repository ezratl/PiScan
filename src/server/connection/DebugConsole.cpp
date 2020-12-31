/*
 * DebugConsole.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: ezra
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

#include "DebugServer.h"
#include "loguru.hpp"
#include "threadname.h"
#include "connection/DebugConsole.h"
#include "constants.h"

namespace piscan {
namespace server {
namespace connection {

#define DS_THREAD_NAME	"DebugConsole"

using namespace piscan;

bool DebugConsole::connect(){
	std::cerr << "\nConnecting...\n";
	_run = true;
	_requestThread = std::thread(&DebugConsole::_consoleInputFunc, this);
	return true;
}

void DebugConsole::disconnect(){
	_run = false;

	std::fprintf(stdin, "\n");
	std::cerr << "\nConsole disconnected\n";
	notifyDisconnected();
}

void DebugConsole::giveMessage(std::shared_ptr<Message> /* message */){

}

void DebugConsole::_consoleInputFunc() {
	setThreadName(DS_THREAD_NAME);

	std::string input = "";
	std::vector<std::string> tokens(5);
	std::stringstream sstream;
	std::string intermediate;
	std::cerr << "\nConsole connected\n";

	getSystemInfo();
	getScannerContext();
	getDemodContext();

	while(_run){
		input.clear();
		std::getline(std::cin, input);
		LOG_F(7, "Command entered: %s", input.c_str());
		sstream = std::stringstream(input);
		while(std::getline(sstream, intermediate, ' '))
			tokens.push_back(intermediate);

		try {
			if (!tokens[0].compare("exit")) {
				_run = false;
				systemFunction(SystemFunction::STOP);
			}
			else if (!tokens[0].compare("verbosity")){
				loguru::g_stderr_verbosity = std::stoi(tokens[1]);
				std::cerr << "Verbosity set to " << loguru::g_stderr_verbosity << std::endl;
			}
			else if (!tokens[0].compare("squelch")) {
				if (tokens.size() > 1) {
					int level = std::stoi(tokens[1]);
					setDemodSquelch(level);
				}
				else
					getDemodContext();
			} else if (!tokens[0].compare("scan"))
				scanStart();
			else if (!tokens[0].compare("hold")) {
				if (tokens.size() > 2) {
					std::vector<int> entryIndex;
					for(size_t i = 1; i < tokens.size(); i++)
						entryIndex.push_back(std::stoi(tokens[i]));
					scanHoldEntry(entryIndex);
				}
				else
					scanHold();
			} else if (!tokens[0].compare("gain")) {
				if (tokens.size() > 1) {
					int gain = 0;
					if (!tokens[1].compare("auto"))
						gain = AUTO_GAIN;
					else
						gain = std::stoi(tokens[1]);
					setDemodGain(gain);
				}
				else
					getDemodContext();
			} else if (!tokens[0].compare("manual")) {
				if(tokens.size() > 2)
					scanManualEntry(std::stof(tokens[1]), tokens[2]);
				else
					scanManualEntry(std::stof(tokens[1]));
			}
			else if (!tokens[0].compare("get")){
				if(!tokens[1].compare("context"))
					getScannerContext();
			}
			else if (!tokens[0].compare("help")) {
				std::cerr << "\n Available commands:"
						<< "\n\thelp\t\tPrints all commands"
						<< "\n\texit\t\tExit program"
						<< "\n\tverbosity [0-9]\tSet console logging verbosity"
						<< "\n\tsquelch [level]\tSet squelch threshold"
						<< "\n\tgain [level]\tSet tuner gain - use \"auto\" for AGC"
						<< "\n\tscan\t\tContinue scanning entries"
						<< "\n\thold\t\tHold scanner at current entry"
						<< "\n\tmanual [freq]\tTune to the specified frequency"
						<< "\n\tget [subcommand]"
						<< "\n\t\tcontext\t\tReturns scanner status"
						<< "\n";
			} else
				std::cerr << "Invalid command\n";
		} catch (std::exception& e) {
			std::cerr << "Argument missing or typo in the command\n";
		}

		sstream.clear();
		tokens.clear();
	}
	std::cerr << "\nConsole thread exited\n";
}

void DebugConsole::contextUpdate(piscan::server::context::ScannerContext context){
	/*std::thread([context]{*/
		switch(context.state){
		case piscan::server::context::ScannerContext::SCAN:
			std::cerr << "\rScanning..." << std::endl;
			break;
		case piscan::server::context::ScannerContext::HOLD:
			std::cerr << "\rHold: " << context.entryIndex << " | "
				<< context.systemTag << " | "
				<< context.entryTag << " | " << (context.frequency / 1E6) << "MHz | "
				<< "LO: " << context.lockout << "\n";
			break;
		case piscan::server::context::ScannerContext::RECEIVE:
			std::cerr << "\rReceive: " << context.entryIndex << " | "
				<< context.systemTag << " | "
				<< context.entryTag << " | " << (context.frequency / 1E6) << "MHz | "
				<< "LO: " << context.lockout << "\n";
			break;
		default:
			break;
		}
	/*});*/
}

void DebugConsole::handleSystemMessage(piscan::server::context::GeneralMessage message){
	static std::map<piscan::server::context::GeneralMessage::MessageType, std::string> messageLabels = {
			{piscan::server::context::GeneralMessage::INFO, "info"},
			{piscan::server::context::GeneralMessage::ERROR, "error"},
			{piscan::server::context::GeneralMessage::WARNING, "warning"}
	};
	std::cerr << messageLabels[message.type];
	/*std::thread([message]{*/
		/*std::cerr << "\rSystem message: ";
		switch(message.type){
		case piscan::server::context::GeneralMessage::INFO:
			std::cerr << "info";
			break;
		case piscan::server::context::GeneralMessage::WARNING:
			std::cerr << "warning";
			break;
		case piscan::server::context::GeneralMessage::ERROR:
			std::cerr << "error";
			break;
		}*/
		std::cerr << "\n" << message.content;
	/*});*/
}

void DebugConsole::contextUpdate(piscan::server::context::DemodContext context) {
	std::cerr << "\rGain: ";
	/*if(context.gain >= 0)
		std::cerr << context.gain;
	else
		std::cerr << "auto";*/
	std::cerr << ((context.gain >= 0) ? std::to_string(context.gain) : "auto");
	std::cerr << "\nSquelch: " << context.squelch << std::endl;
}

void DebugConsole::handleSystemInfo(const piscan::server::context::SystemInfo info){
	std::cerr << "System version: " << info.version << "\n";
	std::cerr << "Build number: " << info.buildNumber << "\n";
	std::cerr << "Squelch range: [" << info.squelchRange.first << ", " << info.squelchRange.second << "\n";
	std::cerr << "Modulatons:\n";
	for(auto mod = info.supportedModulations.begin(); mod != info.supportedModulations.end(); mod++){
		std::cerr << "\t" << *mod << "\n";
	}
}

void DebugConsole::handleSignalLevel(const int level){
	std::cerr << "\r" << "Signal: " << level << "\t\t\t\t";
}

}
}
}
