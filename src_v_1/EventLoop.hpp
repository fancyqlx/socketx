#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP

namespace socketx{

    class EventLoop{
        public:
            EventLoop() = default;
            ~EventLoop();

            /* Main Loop
            *  The Loop will stop when stop falg is set to false
            */
            void loop();

            /*Update events by invoke poller's function*/
            void updateEvent(Event *event);
            void deleteEvent(Event *event);

        private:
            std::vector<Event*> activeEvents;
            socketx::Poller poller;
    };
}
#endif