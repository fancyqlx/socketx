#ifndef POLLER_HPP
#define POLLER_HPP

namespace socket{
    /*Forward declaration*/
    class EventLoop;

    class Poller{
        public:

            /*Wrapper function of Linux/Unix poll
            * Return a vector of active events
            */
            std::vector<Event *> poll();

            /*Update or delete an event from eventList*/
            void updateEvent(Event *event);
            void deleteEvent(Event *event);

        private:
            

            std::vector<Event *> eventList;
            std::vector<struct pollfd> pollfdList;
            std::map<int, Event*> eventMap;

            int timeout_;
    };
}

#endif