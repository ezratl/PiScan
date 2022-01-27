#pragma once

#include <memory>
#include <functional>

namespace piscan {
    namespace events {
        struct Event {
            Event(std::string topic) : topic(topic) {};
            std::string topic;
        };

        typedef std::shared_ptr<Event> EventPtr;
        typedef std::function<void(std::shared_ptr<Event>)> EventHandler;
        //typedef std::shared_ptr<EventHandler> EventHandler;

        void publish(EventPtr event);

        void subscribe(std::string topic, EventHandler handler);
    }
}