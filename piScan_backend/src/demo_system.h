/*
 * demo_system.h
 *
 *  Created on: Mar 5, 2019
 *      Author: ezra
 */

#include "RadioSystem.h"

#ifndef DEMO_SYSTEM_H_
#define DEMO_SYSTEM_H_

class DemoSystem : public AnalogSystem {
public:
	DemoSystem() : AnalogSystem() {
		entries.push_back(new FMChannel(453700000, std::string("Pit Fire 1"), false, true));
		entries.push_back(new FMChannel(453850000, std::string("Pit Fire 2"), false, false));
		entries.push_back(new FMChannel(462950000, std::string("Pit EMS 1"), false, true));
		entries.push_back(new FMChannel(453100000, std::string("Pit PD 1,2"), false, false));
		entries.push_back(new FMChannel(453250000, std::string("Pit PD 3,6"), false, false));
		entries.push_back(new FMChannel(453400000, std::string("Pit PD 4,5"), false, false));
		entries.push_back(new FMChannel(453150000, std::string("Pitt Police"), false, false));
		entries.push_back(new FMChannel(471312500, std::string("East EMS"), false, true));
		entries.push_back(new FMChannel(462450000, std::string("CMU Police"), false, false));
		entries.push_back(new FMChannel(471337500, std::string("South EMS"), false, true));
		entries.push_back(new FMChannel(470437500, std::string("West EMS"), false, true));

	}

	~DemoSystem() {};
};



#endif /* DEMO_SYSTEM_H_ */
