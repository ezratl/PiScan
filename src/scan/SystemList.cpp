/*
 * SystemList.cpp
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#include <experimental/filesystem>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include "SystemList.h"
#include "Configuration.h"
#include "loguru.hpp"
#include "constants.h"

using namespace piscan;
using namespace std::experimental;

std::unordered_map<std::string, std::function<RadioSystemPtr(ptree&, size_t)>> SystemList::_makeSystem = {
		{ANALOG_SYSTEM_HASH, makeAnalogSystem}
};

SystemList::SystemList() {
	// TODO Auto-generated constructor stub

}

SystemList::~SystemList() {
	// TODO Auto-generated destructor stub
}

void SystemList::populateFromFile(){
	filesystem::path path(Configuration::getConfig().getWorkingPath());
	path += filesystem::path::preferred_separator;
	path += SYSTEMS_FILE;
	if(!filesystem::exists(path)){
		LOG_F(INFO, "No system list file found");
		return;
	}
	
	LOG_F(1, "Systems file path %s", path.c_str());
	LOG_F(INFO, "Loading systems file");

	try{
		read_json(path, _jsonTree);
	} catch(boost::property_tree::json_parser_error& e){
		LOG_F(WARNING, "Error parsing file: %s", e.message().c_str());
	}

	BOOST_FOREACH(ptree::value_type& v, _jsonTree.get_child(std::string(SYSTEMS_KEY).append("."))){
		LOG_F(1, "%s", v.first.c_str());
		ptree newSystem = v.second;

		std::string sysType = newSystem.get(SYS_TYPE_KEY, "unspecified");

		RadioSystemPtr system;

		try{
			system = _makeSystem[sysType](newSystem, size());
			LOG_F(1, "Added %s system %s", sysType.c_str(), system->tag());
		} catch (std::exception& e) {
			LOG_F(WARNING, "Unrecognized or unsupported system type: %s", sysType.c_str());
			continue;
		}

		if(system != nullptr)
			addSystem(system);
	}

}

bool SystemList::writeToFile(){
	filesystem::path path(Configuration::getConfig().getWorkingPath());
	path += filesystem::path::preferred_separator;
	path += SYSTEMS_FILE;

	LOG_F(1, "Systems file path", path.c_str());
	LOG_F(INFO, "Saving systems file");

	ptree pt, systems;

	BOOST_FOREACH(RadioSystemPtr p, _systems){
		systems.push_back(std::make_pair("", p->getPropertyTree()));
	}

	pt.add_child(SYSTEMS_KEY, systems);

	write_json(path, pt);

	return true;
}

EntryPtr SystemList::getEntryByIndex(std::vector<int> index){
	if(index.size() < 2 || index[0] < 0 || index[1] < 0 || index[0] >= (long long)_systems.size() || index[1] >= (long long)_systems[index[0]]->size())
		return getNextEntry();

	return _systems[index[0]]->operator [](index[1]);
}

EntryPtr SystemList::getNextEntry(){
	if(_entryNum == 0 && _retune){
		_retune = false;
		//if(_bins[_binNum]->size() > 1)
			return std::make_shared<DummyChannel>(_bins[_binNum]->getCenterFreq());
	}

	auto entry = _bins[_binNum]->at(_entryNum);

	_entryNum = (_entryNum + 1) % _bins[_binNum]->size();

	if(_entryNum == 0){
		_binNum = (_binNum + 1) % _bins.size();
		_retune = true;
	}

	return entry;

}

void SystemList::sortBins(long long bandwidth){
	LOG_F(1, "Sorting bandwidth chunks...");

	size_t numEntries = 0;
	for(size_t i = 0; i < _systems.size(); i++)
		for(size_t k = 0; k < _systems[i]->size(); k++)
			numEntries++;
	EntryPtr entries[numEntries];

	numEntries = 0;

	for(size_t i = 0; i < _systems.size(); i++)
		for(size_t k = 0; k < _systems[i]->size(); k++){
			//entries.push_back(_systems[i]->operator [](k));
			entries[numEntries] = _systems[i]->operator [](k);
			numEntries++;
		}

	mergeSort(entries, 0, numEntries - 1);

	std::shared_ptr<EntryBin> newBin = std::make_shared<EntryBin>();
	_bins.push_back(newBin);
	long lastFreq = 0;
	for(size_t i = 0; i < numEntries; i++){
		if(newBin->size() && entries[i]->freq() >= (newBin->front()->freq() + bandwidth)){
			newBin = std::make_shared<EntryBin>();
			_bins.push_back(newBin);
		}

		if(entries[i]->freq() < lastFreq)
			LOG_F(WARNING, "Entries not sorted properly!");
		lastFreq = entries[i]->freq();
		newBin->push_back(entries[i]);
	}

	std::string binPrint = "";
	for(size_t i = 0; i < _bins.size(); i++){
		binPrint += (std::string("\tCenter: ") + std::to_string(_bins[i]->getCenterFreq()) + std::string(" | "));
		for(size_t j = 0; j < _bins[i]->size(); j++)
			binPrint += (std::to_string(_bins[i]->at(j)->freq()) + std::string(" "));

		RAW_LOG_F(1, binPrint.c_str());
		binPrint = "";
	}

	double totalTimeS = ((numEntries * DEMOD_BUFFER_TIME) + (_bins.size() * (TUNER_RETUNE_TIME + DEMOD_BUFFER_TIME))) / 1000000;
	double totalChannels = numEntries + _bins.size();
	double avgScanRate = totalChannels / totalTimeS;
	LOG_F(1, "Max average scan rate: %.1lf channels per second", avgScanRate);
}

void SystemList::merge(EntryPtr arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    EntryPtr L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];

    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (static_cast<long>(L[i]->freq()) <= static_cast<long>(R[j]->freq()))
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }


}

/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void SystemList::mergeSort(EntryPtr arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);

        merge(arr, l, m, r);
    }
}

long SystemList::EntryBin::getCenterFreq(){
	if(this->size() > 1){
		long range = this->back()->freq() + this->front()->freq();
		return (range) / 2;
	}
	else
		return this->front()->freq();
}

RadioSystemPtr SystemList::makeAnalogSystem(ptree& pt, size_t index){
	auto system = std::make_shared<AnalogSystem>(pt, index);
	return system;
}
