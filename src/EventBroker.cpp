#include "EventBroker.h"
#include "events.h"

namespace piscan {
    namespace events {
        void publish(EventPtr event) {
            EventBroker::instance()->publish(event);
        }

        void subscribe(std::string topic, EventHandler handler){
            EventBroker::instance()->subscribe(topic, std::move(handler));
        }
    }

    std::shared_ptr<EventBroker> EventBroker::_instance = nullptr;
    std::shared_ptr<EventBroker> EventBroker::instance() {
        if(!_instance){
            _instance = std::make_shared<EventBroker>();
        }
        return _instance;
    }

    EventBroker::EventBroker() : WorkerThread("Event Broker") {

    }

    void EventBroker::publish(events::EventPtr event) {
        _eventQueue.enqueue(event);
        postWorkAvailable();
    }

    void EventBroker::subscribe(std::string topic, events::EventHandler handler) {
        if (_handlers.find(topic) == _handlers.end()) {
            _handlers[topic] = std::vector<events::EventHandler>();
        }

        _handlers[topic].push_back(std::move(handler));
    }

    void EventBroker::main() {
        events::EventPtr event;
        if(!_eventQueue.try_dequeue(event)) {
            workAvailable = false;
            return;
        }

        std::string& topic = event->topic;

        // Support for regex subscriptions may be added later
        auto found = _handlers.find(topic);
        if (found != _handlers.end()){
            for (auto handler = _handlers[topic].begin(); handler != _handlers[topic].end(); handler++){
                (*handler)(event);
            }
        }
        // 'All events' subscription
        found = _handlers.find("*");
        if (found != _handlers.end()){
            for (auto handler = _handlers["*"].begin(); handler != _handlers["*"].end(); handler++){
                (*handler)(event);
            }
        }
    }
}