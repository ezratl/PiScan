#pragma once

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace piscan {

class ThreadBase {
public:
    virtual ~ThreadBase() {};

    virtual void initialize();
    virtual void deinitialize();

    virtual void start();
    virtual void stop();

    virtual void join();

    const std::string name;

protected:
    ThreadBase(std::string name);

    virtual void run();
    virtual void main() = 0;
    bool isRunning();
    //Logger logger();

private:
    std::atomic_bool _run;

    std::thread _thread;
};

class WorkerThread : public ThreadBase {
public:
    virtual void start();
    virtual void stop();

protected:
    WorkerThread(std::string name) : ThreadBase(name) {};
    virtual ~WorkerThread(){};

    virtual void run();
    virtual void main() = 0;

    void postWorkAvailable();
    bool workAvailable;
private:
    std::mutex _mtx;
    std::condition_variable _cv;
    
    bool _stopping;
};

}
