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
        std::shared_prt<Connection> conn = std::make_shared(loop_, fd);
        if(!connectionsMap.count(fd)){
            connectionsMap[fd] = conn;
            currentConn = conn;
            /* Run user defined function for new connection*/
            handleConnectionFunc();
        }
        else{
            printf("Error: existing file descriptor for a new connection!");
        }
    }

}