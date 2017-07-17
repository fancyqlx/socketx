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
                auto event = eventMap[it->fd];
                vec.push_back(event);
            }
        }
        return vec;
    }

}