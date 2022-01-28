#include "Thread.h"
#include "threadname.h"
#include "loguru.hpp"

namespace piscan {
    ThreadBase::ThreadBase(std::string name) : name(name), _run(true) {
    }

    void ThreadBase::initialize() {
        LOG_F(1, "Initializing");
    }

    void ThreadBase::deinitialize() {
        LOG_F(1, "Deinitializing");
    }

    void ThreadBase::start() {
        LOG_F(1, "Starting %s", name.c_str());
        _run = true;

        _thread = std::thread([this]() { (*this).run(); });
    }

    void ThreadBase::stop() {
        LOG_F(1, "Stopping %s", name.c_str());
        _run = false;
    }

    void ThreadBase::join() {
        if (_thread.joinable()) {
            _thread.join();
        }
    }

    void ThreadBase::run() {
        setThreadName(name.c_str());

        initialize();
        LOG_F(1, "Started");

        while(isRunning()) {
            main();
        }

        deinitialize();

        LOG_F(1, "Stopped");
    }

    void WorkerThread::start() {
        _stopping = false;
        ThreadBase::start();
    }

    void WorkerThread::stop() {
        ThreadBase::stop();
        std::unique_lock<std::mutex> lock(_mtx);
        _stopping = true;
        _cv.notify_all();
    }

    void WorkerThread::run() {
        setThreadName(name.c_str());

        initialize();
        LOG_F(1, "Started");

        while (isRunning())
        {
            std::unique_lock<std::mutex> lock(_mtx);
            while(!workAvailable && !_stopping) {
                _cv.wait(lock);
            }
            if(workAvailable){
                main();
            }
            else if(_stopping){
                break;
            }
        }

        deinitialize();
        LOG_F(1, "Stopped");
    }

    bool ThreadBase::isRunning() {
        return _run.load();
    }

    void WorkerThread::postWorkAvailable() {
        std::unique_lock<std::mutex> lock(_mtx);
        workAvailable = true;
        _cv.notify_all();
    }
}
