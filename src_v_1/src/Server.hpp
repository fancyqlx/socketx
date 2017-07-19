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
            Server(EventLoop *loop, std::string port);

            void start();

            
            /* Provide an API for users 
            *  to handle new connection
            */
            void setHandleConnectionFunc(std::function<void()> &func){
                handleConnectionFunc = func;
            }
            /* Provide an API for users
            *  to handle events of a connection.
            *  Users need to set these functions in their codes
            *  to regist corresponding events.
            */
            void setHanldeReadEvents(const std::function<void()> &func){
                handleReadEvents = func;
                currentConn->registReadEvents();
            }
            void setHanldeWriteEvents(const std::function<void()> &func){
                handleWriteEvents = func;
                currentConn->registWriteEvents();
            }

            std::shared_ptr<Connection> getCurrentConnection(){
                return currentConn;
            }

        private:

            /* Handle new connections
            * This function is used to create a new object
            * Connection as an event listened by EventLoop
            */
            void newConnection(int fd);

            EventLoop *loop_;
            ServerSocket *socket;

            std::string port_;

            std::map<int, std::shared_ptr<Connection>> connectionsMap;
            std::shared_ptr<Connection> currentConn;

            std::function<void()> handleConnectionFunc;
            std::function<void(Connection *)> handleReadEvents;
            std::function<void(Connection *)> handleWriteEvents;

    };

}
#endif