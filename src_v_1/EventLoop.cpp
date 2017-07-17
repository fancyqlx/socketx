#include "EventLoop.hpp"

namespace socketx{


    /* Main Loop
    *  The Loop will stop when stop falg is set to false
    */
    void EventLoop::loop(){
        while(!stop){
            activeEvents.clear();
            activeEvents = poller.poll();
            for(auto it=activeEvents.begin();it!=activeEvents.end();++it){
                (*it)->handleEvent();
            }
        }
    }

    /*Update events by invoke poller's function*/
    void EventLoop::updateEvent(Event *event){
        poller.updateEvent(event);
    }

    void EventLoop::deleteEvent(Event *event){
        poller.deleteEvent(event);
    }

}