#include "Connection.hpp"

namespace socketx{

    Connection::Connection(EventLoop *loop, int fd):
        loop_(loop),
        socketfd(fd),
        event_(new Event(loop)){
        /*Set callback functions*/
        event_->setReadFunc(std::bind(&Connection::handleRead, this));
        event_->setWriteFunc(std::bind(&Connection::handleWrite, this));
        event_->setErrorFunc(std::bind(&Connection::handleError, this));
        /*Update socketfd*/
        event_->setFD(socketfd);
    }

    void Connection::handleRead(){
        
    }

}