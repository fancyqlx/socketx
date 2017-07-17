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

            void setTimeout(int timeout){
                timeout_ = timeout;
            }

        private:
            

            std::vector<Event *> eventsList;
            std::vector<struct pollfd> pollfdList;
            std::map<int, Event*> eventsMap;

            int timeout_;
    };
}

#endif