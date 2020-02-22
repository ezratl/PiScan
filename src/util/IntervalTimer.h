/*
 * IntervalTimer.h
 *
 *  Created on: Feb 12, 2020
 *      Author: ezra
 */

#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include <iostream>

namespace piscan {

using std::thread;
using std::mutex;
using std::condition_variable;
using std::atomic_bool;
using std::function;
using std::unique_lock;
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

class IntervalTimer {
public:
	IntervalTimer(): _runTimer(false), _execute(false) {};
	~IntervalTimer(){
		if(_execute.load())
			destroy();
	};

	inline void create(long long interval, function<void()> func){
		_execute = true;
		_runTimer = false;
		_ready = false;

		_timerThread = thread([this, interval, func](){
			while(_execute){
				{
					unique_lock<std::mutex> lock(_mtx);
					while(!_runTimer.load() && _execute.load()){
						_cv.wait(lock);
					}
					lock.unlock();

					if(!_execute.load()) break;
				}

				auto periodStart = high_resolution_clock::now();

				func();

				{
					milliseconds remainingSleep(interval);
					bool sleep = true;

					unique_lock<std::mutex> lock(_sleepMtx);

					while(sleep){
						if(_sleepTime.wait_for(lock, remainingSleep, [this]{ return _stateChanged.load();})){
							//std::cout << "\t\ttimer state changed\n";
							_stateChanged = false;
							break;
						}
						// should account for spurious wakeup
						else{
							//std::cout << "\t\tstate change false\n";
							auto elapsed = high_resolution_clock::now() - periodStart;
							remainingSleep -= duration_cast<milliseconds>(elapsed);
							//std::cout << "\t\t" << remainingSleep.count() << "\n";
							if(remainingSleep.count() <= 0){
								sleep = false;
								//std::cout << "\t\ttimed out\n";
							}
						}
					}
					//std::cout << "\t\twoken up\n";
				}
			}
		});
		_timerThread.detach();

	}

	inline void start(){
        unique_lock<mutex> lock(_mtx);
        _runTimer = true;
        _stateChanged = false;
        _cv.notify_all();
        //unique_lock<mutex> lock2(_sleepMtx);
        //_sleepTime.notify_all();

	}

	inline void stop(){
		//unique_lock<mutex> lock(_mtx);
		_runTimer = false;
		_stateChanged = true;
		unique_lock<mutex> lock2(_sleepMtx);
		_sleepTime.notify_all();
	}

private:
	inline void destroy(){
		//std::cout << "\tdestroy timer\n";
		_execute = false;
		unique_lock<mutex> lock(_mtx);
		_cv.notify_all();
		stop();
	}

private:
	thread _timerThread;
	atomic_bool _runTimer;
	atomic_bool _execute;
	atomic_bool _stateChanged{false};

	mutex _mtx;
	condition_variable _cv;
	mutex _sleepMtx;
	condition_variable _sleepTime;
	bool _ready = false;
};

} /* namespace piscan */

