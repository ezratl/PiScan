/*
 * SystemList.h
 *
 *  Created on: Oct 21, 2018
 *      Author: ezra
 */

#ifndef SYSTEMLIST_H_
#define SYSTEMLIST_H_

#include <list>

#include "RadioSystem.h"

class SystemList {
public:
	SystemList();
	virtual ~SystemList();



private:
	std::list<RadioSystem> systems;
};

#endif /* SYSTEMLIST_H_ */
