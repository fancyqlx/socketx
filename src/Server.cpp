#include "Server.hpp"


namespace socketx{

    Server::Server(EventLoop *loop, std::string port):
        loop_(loop),
        port_(port),
        socket_(new ServerSocket(loop, port)){
            socket_->setNewConnctionFunc(std::bind(&Server::newConnection, this, std::placeholders::_1));
        }

    Server::~Server(){
        for(auto it=connectionsMap.begin();it!=connectionsMap.end();++it){
            it->handleClose();
        }
    }
 
    void Server::start(){
        socket_->listen();
    }

    void Server::newConnection(int fd){
        std::shared_ptr<Connection> conn = std::make_shared<Connection>(loop_, fd);
        if(!connectionsMap.count(fd)){
            connectionsMap[fd] = conn;
            currentConn = conn;
            conn->setHandleCloseEvents(std::bind(&Server::removeConnection, this, std::placeholders::_1));
            /* Run user defined function for new connection*/
            handleConnectionFunc(conn);
        }
        else{
            printf("Error: existing file descriptor for a new connection!\n");
        }
    }

    void Server::removeConnection(std::shared_ptr<Connection> conn){
        int fd = conn->getFD();
        auto it = connectionsMap.find(fd);
        connectionsMap.erase(it);
        /*Run user defined function for closing connection*/
        handleCloseEvents(conn);
    }

}