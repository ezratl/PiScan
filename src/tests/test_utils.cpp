/*
 * test_utils.cpp
 *
 *  Created on: Feb 12, 2020
 *      Author: ezra
 */

#include <iostream>
#include <functional>
#include <chrono>

#include "IntervalTimer.h"

using piscan::IntervalTimer;
using std::cout;
using std::chrono::milliseconds;

int main(int argc, char** argv){
	(void) argc;
	(void) argv;

	cout << "testing IntervalTimer" << std::endl;
	size_t runCount = 0;
	auto func = std::function<void(void)>([&runCount](){
		runCount++;
		cout << "\ttimer running\n";
	});

	IntervalTimer it;
	cout << "create 10ms timer\n";
	it.create(10, func);
	cout << "start timer\n";
	it.start();
	cout << "waiting 50ms\n";
	std::this_thread::sleep_for(milliseconds(50));
	cout << "stopping timer\n";
	it.stop();
	cout << "timer executed " << runCount << " times\n";

	IntervalTimer& it2 = *(new IntervalTimer());
	cout << "recreating timer, 15ms\n";
	runCount = 0;
	it2.create(15, func);
	cout << "waiting 50ms before starting\n";
	std::this_thread::sleep_for(milliseconds(50));
	cout << "start timer\n";
	it2.start();
	cout << "waiting 50ms\n";
	std::this_thread::sleep_for(milliseconds(50));
	cout << "destroy while timer is running\n";
	delete &it2;
	cout << "timer destroyed\n";

	IntervalTimer it3;
	cout << "recreating timer, 20ms\n";
	runCount = 0;
	it3.create(20, func);
	cout << "start timer, 2 runs\n";
	it3.start();
	cout << "wait 50ms\n";
	std::this_thread::sleep_for(milliseconds(50));
	cout << "stop timer\n";
	it3.stop();
	cout << "ran " << runCount << " times. wait 50ms\n";
	std::this_thread::sleep_for(milliseconds(50));
	cout << "restart timer\n";
	it3.start();
	cout << "wait 50ms\n";
	std::this_thread::sleep_for(milliseconds(50));
	cout << "stop timer\n";
	it3.stop();
	cout << "total " << runCount << " runs\n";

	return 0;
}

