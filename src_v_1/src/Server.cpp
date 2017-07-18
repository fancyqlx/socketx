#include "Server.hpp"


namespace socketx{

    Server::Server(EventLoop *loop, std::string port):
        loop_(loop),
        port_(port),
        socket(new ServerSocket(loop, port)){
            socket->setNewConnctionFunc(std::bind(&Server::newConnection,this,_1));
        }

    void Server::start(){
        socket->listen();
    }

    void Server::newConnection(int fd){
        Connection *conn = new Connection(loop_, fd);
        if(!connectionsMap.count(fd)){
            connectionsMap[fd] = conn;
            /*Set user defined function for reading or writing*/
            conn->setHandleEventsFunc(handleEventsFunc);
            /*Regist events*/
            conn->initConnection();
            /* Run user defined function for new connection*/
            handleConnectionFunc();
        }
        else{
            printf("Error: existing file descriptor for a new connection!");
        }
    }

}