/*
 * SentinelFile.h
 *
 *  Created on: Mar 14, 2019
 *      Author: ezra
 */

#ifndef SCAN_SENTINELFILE_H_
#define SCAN_SENTINELFILE_H_

#include <vector>
#include <string>

#include "SystemList.h"

namespace piscan {

class SystemList;

class ListFileIO {
public:
	ListFileIO() {}
	virtual ~ListFileIO() {}

	virtual bool generateSystemList(SystemList& list) = 0;
};

class SentinelFile : public ListFileIO {
public:
	SentinelFile(std::string path) : _path(path) {};
	~SentinelFile() {};

	bool generateSystemList(SystemList& list);

private:
	SystemList* _list = nullptr;
	std::shared_ptr<RadioSystem> _system;
	std::string _path;

	void _newAnalogSys(std::vector<std::string>& tokens);
	void _newAnalogEntry(std::vector<std::string>& tokens);
};

}

#endif /* SCAN_SENTINELFILE_H_ */
