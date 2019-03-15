/*
 * ServerDebugOutput.cpp
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "constants.h"
#include "DebugServer.h"
#include "loguru.hpp"

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

void DebugConsole::giveMessage(Message& message){
	delete &message;
}

void DebugConsole::_consoleInputFunc() {
	std::string input = "";
	std::vector<std::string> tokens(5);
	std::stringstream sstream;
	std::string intermediate;
	std::cerr << "\nConsole connected\n";
	while(_run){
		input.clear();
		std::getline(std::cin, input);
		sstream = std::stringstream(input);
		while(std::getline(sstream, intermediate, ' '))
			tokens.push_back(intermediate);

		try {
			if (tokens[0].compare("exit") == 0) {
				_run = false;
				systemFunction(SystemFunction::STOP);
			} else if (tokens[0].compare("squelch") == 0) {
				int level = std::stoi(tokens[1]);
				setDemodSquelch(level);
			} else if (tokens[0].compare("scan") == 0)
				scannerFunction(ScannerFunction::SCAN);
			else if (tokens[0].compare("hold") == 0) {
				scannerFunction(ScannerFunction::HOLD);
			} else if (tokens[0].compare("gain") == 0) {
				int gain = 0;
				if (tokens[1].compare("auto") == 0)
					gain = AUTO_GAIN;
				else
					gain = std::stoi(tokens[1]);
				setDemodGain(gain);
			} else if (tokens[0].compare("manual") == 0) {
				scannerFunction(ScannerFunction::MANUAL, std::stof(tokens[1]));
			}
			else if (tokens[0].compare("get") == 0){
				if(tokens[1].compare("context") == 0)
					getCurrentContext();
			}
			else if (tokens[0].compare("help") == 0) {
				std::cerr << "\n Available commands:"
						<< "\n\thelp\t\tPrints all commands"
						<< "\n\texit\t\tExit program"
						<< "\n\tsquelch [level]\tSet squelch threshold"
						<< "\n\tgain [level]\tSet tuner gain - use \"auto\" for AGC"
						<< "\n\tscan\t\tContinue scanning entries"
						<< "\n\thold\t\tHold scanner at current entry"
						<< "\n\tmanual [freq]\tTune to the specified frequency"
						<< "\n\tget [subcommand]"
						<< "\n\t\t\tcontext"
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

void DebugConsole::contextUpdate(ScannerContext context){
	/*std::thread([context]{*/
		switch(context.state()){
		case ScannerContext::SCAN:
			std::cerr << "\rScanning: " << context.systemTag() << "\n";
			break;
		case ScannerContext::HOLD:
			std::cerr << "\rHold: " << context.entryIndex() << " | "
				<< context.systemTag() << " | "
				<< context.entryTag() << " | " << (context.frequency() / 1E6) << "MHz\n";
			break;
		case ScannerContext::RECEIVE:
			std::cerr << "\rReceive: " << context.entryIndex() << " | "
				<< context.systemTag() << " | "
				<< context.entryTag() << " | " << (context.frequency() / 1E6) << "MHz\n";
			break;
		default:
			break;
		}
	/*});*/
}

void DebugConsole::systemMessage(GeneralMessage message){
	/*std::thread([message]{*/
		std::cerr << "\rSystem message: ";
		switch(message.type()){
		case GeneralMessage::INFO:
			std::cerr << "info";
			break;
		case GeneralMessage::WARNING:
			std::cerr << "warning";
			break;
		case GeneralMessage::ERROR:
			std::cerr << "error";
			break;
		}
		std::cerr << "\n" << message.content();
	/*});*/
}

void DebugConsole::gainReceived(int handle, int gain) {
	std::cerr << "\rGain: ";
	if(gain >= 0)
		std::cerr << gain;
	else
		std::cerr << "auto";
	std::cerr << "\n";
}

void DebugServer::start(){
	this->_connection = new DebugConsole();
	this->_host.requestConnection(_connection);
	if(_connection == nullptr)
		DLOG_F(WARNING, "Debug connection failed");
}

void DebugServer::stop(){
	if (_connection->_run)
		_connection->disconnect();
}

void DebugServer::giveMessage(Message& message){
	delete &message;
}

