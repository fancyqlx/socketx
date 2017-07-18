#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "Event.hpp"
#include "Socket.hpp"

namespace socketx{

    /* Forward declaration*/

    class Connection{
        public:
            Connection(EventLoop *loop, int sockfd);
            ~Connection();
            
            /*Regist events*/
            void initConnection();

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
            Socket *socket_;

            std::function<void()> handleEventsFunc;

    };


}


#endif