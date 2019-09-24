/*
 * SystemList.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef SYSTEMLIST_H_
#define SYSTEMLIST_H_

#include <vector>

#include "RadioSystem.h"

namespace piscan {

class SystemList {
public:
	SystemList();
	~SystemList();

	std::shared_ptr<RadioSystem> operator[](size_t pos) { return _systems[pos]; };

	size_t size(){ return _systems.size(); }

	void addSystem(std::shared_ptr<RadioSystem> system) {
		_systems.push_back(system);
	}

	std::shared_ptr<Entry> getNextEntry();

	void sortBins(int bandwidth);
private:

	class EntryBin: public std::vector<std::shared_ptr<Entry>>{
	public:
		EntryBin(){};
		~EntryBin(){};

		void sort();
		long getCenterFreq();
	};


	std::vector<std::shared_ptr<RadioSystem>> _systems;
	//size_t _size = 0;

	std::vector<std::shared_ptr<EntryBin>> _bins;

	size_t _binNum = 0, _entryNum = 0;
	bool _retune = true;

	void merge(std::shared_ptr<Entry> arr[], int l, int m, int r);
	void mergeSort(std::shared_ptr<Entry> arr[], int l, int r);
};
}
#endif /* SYSTEMLIST_H_ */
