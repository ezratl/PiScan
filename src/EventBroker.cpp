#include <tuple>

#include "EventBroker.h"
#include "events.h"
#include "loguru.hpp"

namespace piscan {
    namespace events {
        void publish(EventPtr event) {
            EventBroker::instance()->publish(event);
        }

        void subscribe(std::string topic, int subscriber, EventHandler handler){
            EventBroker::instance()->subscribe(topic, subscriber, std::move(handler));
        }

        void unsubscribe(std::string topic, int subscriber){
            EventBroker::instance()->unsubscribe(topic, subscriber);
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

    void EventBroker::subscribe(std::string topic, int subscriber, events::EventHandler handler) {
        std::unique_lock<std::mutex> lock(_handler_mutex);
        if (_handlers.find(topic) == _handlers.end()) {
            _handlers[topic] = std::map<int, events::EventHandler>();
        }

        _handlers[topic][subscriber] = std::move(handler);
    }

    void EventBroker::unsubscribe(std::string topic, int subscriber) {
        std::unique_lock<std::mutex> lock(_handler_mutex);
        _handlers[topic].erase(subscriber);
    }

    void EventBroker::main() {
        events::EventPtr event;
        if(!_eventQueue.try_dequeue(event)) {
            workAvailable = false;
            return;
        }

        std::string& topic = event->topic;

        std::unique_lock<std::mutex> lock(_handler_mutex);
        // Support for regex subscriptions may be added later
        auto found = _handlers.find(topic);
        if (found != _handlers.end()){
            for (auto handler = _handlers[topic].begin(); handler != _handlers[topic].end(); handler++){
                try {
                    if ((*handler).second == nullptr) {
                        _handlers[topic].erase((*handler).first);
                        continue;
                    }
                    (*handler).second(event);
                } catch (std::exception& e) {
                    LOG_F(WARNING, "Invalid event handler encountered, deleting");
                    _handlers[topic].erase((*handler).first);
                }
            }
        }
        // 'All events' subscription
        found = _handlers.find("*");
        if (found != _handlers.end()){
            for (auto handler = _handlers["*"].begin(); handler != _handlers["*"].end(); handler++){
                try {
                    if ((*handler).second == nullptr) {
                        _handlers[topic].erase((*handler).first);
                        continue;
                    }
                    (*handler).second(event);
                } catch (std::exception& e) {
                    LOG_F(WARNING, "Invalid event handler encountered, deleting");
                    _handlers[topic].erase((*handler).first);
                }
            }
        }
    }
}