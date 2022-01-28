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
    moodycamel::ConcurrentQueue<std::tuple<std::string, int, events::EventHandler>> _subscribeQueue;
    moodycamel::ConcurrentQueue<std::tuple<std::string, int>> _unsubscribeQueue;

    std::map<std::string, std::map<int, events::EventHandler>> _handlers;

    void _subscribe(std::string topic, int subscriber, events::EventHandler handler);
    void _unsubscribe(std::string topic, int subscriber);
};

}
