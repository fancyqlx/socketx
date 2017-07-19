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
            void setHandleConnectionFunc(const std::function<void()> &func){
                handleConnectionFunc = func;
            }
            /* Provide an API for users
            *  to handle events of a connection.
            *  Users need to set these functions in their codes
            *  to regist corresponding events.
            */
            void setHandleReadEvents(const std::function<void(Connection*)> &func){
                handleReadEvents = func;
                currentConn->setHandleReadEvents(handleReadEvents);
                currentConn->registReadEvents();
            } 
            void setHandleWriteEvents(const std::function<void(Connection*)> &func){
                handleWriteEvents = func;
                currentConn->setHandleWriteEvents(handleWriteEvents);
                currentConn->registWriteEvents();
            }

            /* This function need reture the current connection*/
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
            ServerSocket *socket_;

            std::string port_;

            std::map<int, std::shared_ptr<Connection>> connectionsMap;
            std::shared_ptr<Connection> currentConn;

            std::function<void()> handleConnectionFunc;
            std::function<void(Connection *)> handleReadEvents;
            std::function<void(Connection *)> handleWriteEvents;

    };

}
#endif