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

class SystemList {
public:
	SystemList();

	RadioSystem& operator[](size_t pos);

	size_t size(){ return _size; }
private:
	std::vector<RadioSystem*> _systems;
	size_t _size = 0;
};

#endif /* SYSTEMLIST_H_ */
