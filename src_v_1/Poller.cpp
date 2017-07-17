#include "Poller.hpp"

namespace socketx{

    /*Wrapper function of Linux/Unix poll
    * Return a vector of active events
    */
    std::vector<Event *> Poller::poll(){
        int num = ::poll(pollfdList.begin(),pollfdList.size(),timeout_);
        
        /*Construct active events vector*/
        vector<Event *> vec;
        for(auto it=pollfdList.begin();it!=pollfdList.end()++it){
            if(it->revents>0){
                auto events = eventMap[it->fd];
                events->setRevent(it->revents); 
                vec.push_back(events);
            }
        }
        return vec;
    }

    /*Update or delete an event from eventList*/
    void Poller::updateEvent(Event *event){
        if(eventsMap.count(event->getFD())){
            
        }
    }
    void Poller::deleteEvent(Event *event){

    }
}