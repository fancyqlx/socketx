#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Event.hpp"
#include "Socket.hpp"

namespace socketx{

    /* Forward declaration*/

    class Connection:public Socket{
        public:
            Connection(EventLoop *loop, int fd);
            ~Connection();
            
            /*Regist events*/
            void registReadEvnets(){
                event_->enableReading();
            }
            void registWriteEvnets(){
                event_->enableWriting();
            }


            /*Handle events*/
            void handleRead();
            void handleWrite();
            void handleError();

            /* Set user defined function*/
            void setHandleEventsFunc(std::function<void()> &func){
                handleEventsFunc = func;
            }

            /*Send and write functions*/


        private:
            Event *event_;
            EventLoop *loop_;

            std::function<void()> handleEventsFunc;

    };


}


#endif