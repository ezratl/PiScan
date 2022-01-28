#pragma once

#include <memory>
#include <functional>

namespace piscan {
    namespace events {
        struct Event {
            Event(std::string topic) : topic(topic) {};
            virtual ~Event(){};
            std::string topic;
        };

        typedef std::shared_ptr<Event> EventPtr;
        typedef std::function<void(std::shared_ptr<Event>)> EventHandler;
        //typedef std::shared_ptr<EventHandler> EventHandler;

        void publish(EventPtr event);

        void subscribe(std::string topic, EventHandler handler);

        /*---------------------------------*/
        struct ScannerStateEvent : public Event {
            virtual ~ScannerStateEvent() {};

            enum ScannerState {
                OTHER_STATE,
                SCAN,
                HOLD,
                RECEIVE,
            };

            ScannerStateEvent() : Event("scanner_state_change") {};

            //TODO ported from context messages
            ScannerState state = OTHER_STATE;
            std::string systemTag = "";
            std::string entryTag = "";
            long long frequency = 0;
            std::string modulation = "";
            std::string entryIndex = "";
            int delayMS = 0;
            bool lockout = false;
        };

        struct DemodStateEvent : public Event {
            DemodStateEvent() : Event("demod_state_change") {};
            virtual ~DemodStateEvent() {};
            float tunerGainState;
            int squelchState;
        };

        struct SignalLevelEvent : public Event {
            SignalLevelEvent(int level) : Event("signal_level"), level(level) {};
            virtual ~SignalLevelEvent(){};
            int level;
        };

        struct LoggingEvent : public Event {
            LoggingEvent(int level, std::string line) : Event("log_write"), level(level), line(line) {};
            virtual ~LoggingEvent(){};
            int level;
            std::string line;
        };
    }
}