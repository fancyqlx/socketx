#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP

#include "utilx.hpp"

#include "Event.hpp"
#include "Poller.hpp"


namespace socketx{

    class EventLoop{
        public:
            EventLoop();
            ~EventLoop();

            /* Main Loop
            *  The Loop will stop when stop falg is set to false
            */
            void loop();
            void quit(){
                stop = true;
            }

            /*Update events by invoke poller's function*/
            void updateEvent(Event *event);
            void deleteEvent(Event *event);

        private:
            std::vector<Event*> activeEvents;
            socketx::Poller *poller;
            std::atomic<bool> stop;
    };
}
#endif