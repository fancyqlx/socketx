#ifndef SERVER_HPP
#define SERVER_HPP

#include "Socket.hpp"
#include "Connection.hpp"
#include "utilx.hpp"

namespace socketx{

    /* Forward declaration*/
    class EventLoop;

    class Server{
        public:
            Server(EventLoop *loop);

            void start();

            
            /* Provide an API for users 
            *  to handle new connection
            */
            void setHandleConnectionFunc(std::function<void()> &func){
                handleConnectionFunc = func;
            }
            /* Provide an API for users
            *  to handle events of a connection
            */
            void setHanldeEventsFunc(std::function<void()> &func){
                handleEventsFunc = func;
            }

        private:

            /* Handle new connections
            * This function is used to create a new object
            * Connection as an event listened by EventLoop
            */
            void newConnection();

            EventLoop *loop_;
            ServerSocket *socket;

            std::map<int,Connection *> connectionsMap;

            std::function<void()> handleConnectionFunc;
            std::function<void()> handleEventsFunc;

    };

}
#endif