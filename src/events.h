#pragma once

#include <memory>
#include <functional>

namespace piscan {
    namespace events {
        struct Event {
            Event(std::string topic) : topic(topic) {};
            virtual ~Event(){};

            inline virtual std::string to_string() {
                return "Event type: " + topic + "\n";
            }
            
            std::string topic;
        };

        typedef std::shared_ptr<Event> EventPtr;
        typedef std::function<void(std::shared_ptr<Event>)> EventHandler;
        //typedef std::shared_ptr<EventHandler> EventHandler;

        void publish(EventPtr event);

        void subscribe(std::string topic, int subscriber, EventHandler handler);

        void unsubscribe(std::string topic, int subscriber);

        /*---------------------------------*/
        struct GenericStringEvent : public Event {
            GenericStringEvent(std::string topic, std::string data) : Event(topic), data(data) {};
            virtual ~GenericStringEvent() {};
            inline virtual std::string to_string() {
                return Event::to_string() + "\tData:\t" + data + "\n";
            }
            std::string data;
        };

        struct GenericNumberEvent : public Event {
            GenericNumberEvent(std::string topic, long long data) : Event(topic), data(data) {};
            virtual ~GenericNumberEvent() {};
            inline virtual std::string to_string() {
                return Event::to_string() + "\tData:\t" + std::to_string(data) + "\n";
            }
            long long data;
        };

        struct GenericBoolEvent : public Event {
            GenericBoolEvent(std::string topic, bool data) : Event(topic), data(data) {};
            virtual ~GenericBoolEvent() {};
            inline virtual std::string to_string() {
                return Event::to_string() + "\tData:\t" + std::to_string(data) + "\n";
            }
            bool data;
        };

        struct ScannerStateEvent : public Event {
            virtual ~ScannerStateEvent() {};

            enum ScannerState {
                OTHER_STATE,
                SCAN,
                HOLD,
                RECEIVE,
            };

            ScannerStateEvent() : Event("scanner_state_change") {};

            inline virtual std::string to_string() {
                std::string tail = (state == HOLD || state == RECEIVE)?
                "\tSystem:\t" + systemTag + "\n" +
                "\tEntry:\t" + entryTag + "\n" +
                "\tFrequency:\t" + std::to_string(frequency) + "\n" +
                "\tModulation:\t" + modulation + "\n" + 
                "\tIndex:\t" + entryIndex + "\n" +
                "\tDelay:\t" + std::to_string(delayMS) + "\n" +
                "\tLockout:\t" + std::to_string(lockout) + "\n"
                : "";
                return Event::to_string() + 
                "\tState:\t" + std::to_string(state) + "\n" + tail;
            }

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
            inline virtual std::string to_string() {
                return Event::to_string() + 
                "\tTuner gain:\t" + std::to_string(tunerGainState) + "\n"
                "\tSquelch:\t" + std::to_string(squelchState) + "\n";
            }
            float tunerGainState;
            int squelchState;
        };

        struct SignalLevelEvent : public Event {
            SignalLevelEvent(int level) : Event("signal_level"), level(level) {};
            virtual ~SignalLevelEvent(){};
            inline virtual std::string to_string() {
                return Event::to_string() + "\tSignal level:\t" + std::to_string(level) + "\n";
            }
            int level;
        };

        struct LoggingEvent : public Event {
            LoggingEvent(int level, std::string line) : Event("log_write"), level(level), line(line) {};
            virtual ~LoggingEvent(){};
            inline virtual std::string to_string() {
                return Event::to_string() + "\t> " + line + "\n";
            }
            int level;
            std::string line;
        };
    }
}