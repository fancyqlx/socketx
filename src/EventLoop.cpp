#include "EventLoop.hpp"

namespace socketx{

    EventLoop::EventLoop():
        poller(new Poller()),stop(false){
            IgnoreSIGPIPE();
        }

    EventLoop::~EventLoop(){

    }

    /* Main Loop
    *  The Loop will stop when stop falg is set to false
    */
    void EventLoop::loop(){
        printf("EventLoop starts......\n");
        while(!stop){
            activeEvents.clear();
            activeEvents = poller->poll();
            for(auto it=activeEvents.begin();it!=activeEvents.end();++it){
                (*it)->handleEvent();
            }
        }
        printf("EventLoop ends......\n");
    }

    /*Update events by invoke poller's function*/
    void EventLoop::updateEvent(Event *event){
        poller->updateEvent(event);
    }

    void EventLoop::deleteEvent(Event *event){
        poller->deleteEvent(event);
    }

}