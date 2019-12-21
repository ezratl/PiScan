/*
 * SystemList.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef SYSTEMLIST_H_
#define SYSTEMLIST_H_

#include <vector>
#include <unordered_map>
#include <boost/property_tree/ptree.hpp>

#include "RadioSystem.h"
#include "Entry.h"

#define SYSTEMS_KEY	"systems"

namespace piscan {

class SystemList {
public:
	SystemList();
	~SystemList();

	void populateFromFile();
	bool writeToFile();

	RadioSystemPtr operator[](size_t pos) { return _systems[pos]; };

	size_t size(){ return _systems.size(); }

	void addSystem(RadioSystemPtr system) {
		_systems.push_back(system);
	}

	EntryPtr getEntryByIndex(std::vector<int> index);

	EntryPtr getNextEntry();

	void sortBins(long long bandwidth);
private:

	class EntryBin: public std::vector<EntryPtr>{
	public:
		EntryBin(){};
		~EntryBin(){};

		void sort();
		long getCenterFreq();
	};

	ptree _jsonTree;
	static std::unordered_map<std::string, std::function<RadioSystemPtr(ptree&, size_t)>> _makeSystem;

	std::vector<RadioSystemPtr> _systems;
	//size_t _size = 0;

	std::vector<std::shared_ptr<EntryBin>> _bins;

	size_t _binNum = 0, _entryNum = 0;
	bool _retune = true;

	void merge(EntryPtr arr[], int l, int m, int r);
	void mergeSort(EntryPtr arr[], int l, int r);

	static RadioSystemPtr makeAnalogSystem(ptree& pt, size_t index);
};
}
#endif /* SYSTEMLIST_H_ */
