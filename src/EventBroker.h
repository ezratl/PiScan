#pragma once

#include <memory>
#include <map>
#include <vector>
#include <tuple>

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
    void subscribe(std::string topic, int subscriber, events::EventHandler handler);
    void unsubscribe(std::string topic, int subscriber);

protected:
    //EventBroker();

    void main();

private:
    static std::shared_ptr<EventBroker> _instance;

    moodycamel::ConcurrentQueue<events::EventPtr> _eventQueue;

    std::map<std::string, std::map<int, events::EventHandler>> _handlers;
    std::mutex _handler_mutex;

    void _subscribe(std::string topic, int subscriber, events::EventHandler handler);
    void _unsubscribe(std::string topic, int subscriber);
};

}
