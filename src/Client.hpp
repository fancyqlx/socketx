#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "utilx.hpp"
#include "Socket.hpp"
#include "Connection.hpp"

namespace socketx{

    class Client{
        public:
            Client(EventLoop *loop, std::string hostname, std::string port);
            
            /*Start to connect to a host. 
            * This function should retry the connection
            * until the connection is established or terminated by users.
            */
            void start();

            /* Provide an API for users 
            *  to handle new connections
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
            std::string hostname_;
            std::string port_;
            ClientSocket * socket_;
            std::shared_ptr<Connection> currentConn;

            std::function<void()> handleConnectionFunc;
            std::function<void(Connection*)> handleReadEvents;
            std::function<void(Connection*)> handleWriteEvents;
    };

}


#endif