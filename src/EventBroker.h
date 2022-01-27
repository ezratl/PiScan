#pragma once

#include <memory>
#include <map>
#include <vector>

#include "Thread.h"
#include "events.h"
#include "concurrentqueue.h"

namespace piscan {

class EventBroker : public WorkerThread {
public:
    static std::shared_ptr<EventBroker> instance();

    EventBroker();
    virtual ~EventBroker(){};

    void publish(events::EventPtr event);
    void subscribe(std::string topic, events::EventHandler handler);

protected:
    //EventBroker();

    void main();

private:
    static std::shared_ptr<EventBroker> _instance;

    moodycamel::ConcurrentQueue<events::EventPtr> _eventQueue;

    std::map<std::string, std::vector<events::EventHandler>> _handlers;
};

}
