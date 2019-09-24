/*
 * SystemList.cpp
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#include "SystemList.h"

using namespace piscan;

SystemList::SystemList() {
	// TODO Auto-generated constructor stub

}

SystemList::~SystemList() {
	// TODO Auto-generated destructor stub
}


std::shared_ptr<Entry> SystemList::getNextEntry(){
	if(_entryNum == 0 && _retune){
		_retune = false;
		return std::make_shared<DummyChannel>(_bins[_binNum]->getCenterFreq());
	}

	auto entry = _bins[_binNum]->operator [](_entryNum);

	_entryNum = (_entryNum + 1) % _bins[_binNum]->size();

	if(_entryNum == 0){
		_binNum = (_binNum + 1) % _bins.size();
		_retune = true;
	}

	return entry;

}

void SystemList::sortBins(int bandwidth){
	size_t numEntries = 0;
	for(size_t i = 0; i < _systems.size(); i++)
		for(size_t k = 0; k < _systems[i]->size(); k++)
			numEntries++;
	std::shared_ptr<Entry> entries[numEntries];

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
}

void SystemList::merge(std::shared_ptr<Entry> arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* create temp arrays */
    std::shared_ptr<Entry> L[n1], R[n2];

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
void SystemList::mergeSort(std::shared_ptr<Entry> arr[], int l, int r)
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
