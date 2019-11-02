#pragma once

#include <mutex>
#include <condition_variable>

using namespace std;

class Synchronizable {
    public:
    Synchronizable(){};
    virtual ~Synchronizable(){};

    inline void waitReady(){
        unique_lock<mutex> lock(_mtx);
        while(!_ready)
            _cv.wait(lock);
    }

    inline void waitDeinit(){
        unique_lock<mutex> lock(_mtx);
        while(!_stopped)
            _cv.wait(lock);
    }

    protected:
    inline void notifyReady(){
        unique_lock<mutex> lock(_mtx);
        _ready = true;
        _stopped = false;
        _cv.notify_all();
    }

    inline void notifyDeinit(){
        unique_lock<mutex> lock(_mtx);
        _ready = false;
        _stopped = true;
        _cv.notify_all();
    }

    private:
    mutex _mtx;
    condition_variable _cv;
    bool _ready;
    bool _stopped;
};
