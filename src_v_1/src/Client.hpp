#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "utilx.hpp"
#include "Socket.hpp"

namespace socketx{

    class Client{

        Client(EventLoop *loop, std::string hostname, std::string port);
        
        /*Start to connect to a host. 
        * This function should retry the connection
        * until the connection is established or terminated by users.
        */
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
        void setHanldeReadEvents(const std::function<void(Connection*)> &func){
            handleReadEvents = func;
            currentConn->setHanldeReadEvents(std::bind(&Server::handleReadEvents,this,_1));
            currentConn->registReadEvents();
        }
        void setHanldeWriteEvents(const std::function<void(Connection*)> &func){
            handleWriteEvents = func;
            currentConn->setHanldeWriteEvents(std::bind(&Server::handleWriteEvents,this,_1));
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
        Connection * currentConn;

        std::function<void()> handleConnectionFunc;
        std::function<void(Connection *)> handleReadEvents;
        std::function<void(Connection *)> handleWriteEvents;
    }

}


#endif