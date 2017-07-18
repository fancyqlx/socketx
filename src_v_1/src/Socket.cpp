#include "Socket.hpp"

namespace socketx{

    ServerSocket::ServerSocket(EventLoop *loop, std::string port):
        loop_(loop),
        port_(port),
        event_(new Event(loop)){
            event_->setReadFunc(std::bind(&ServerSocket::handleAccept,this));
        }

    int ServerSocket::listen(){

        /*  Invoke C API for listening a port*/
        struct addrinfo *listp, *p;
        int rc, optval=1;

        const char *port = port_.c_str();

        /* Get a list of potential server addresses */
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_socktype = SOCK_STREAM;             /* Accept connections */
        hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* ... on any IP address */
        hints.ai_flags |= AI_NUMERICSERV;            /* ... using port number */
        if ((rc = getaddrinfo(NULL, port, &hints, &listp)) != 0) {
            fprintf(stderr, "getaddrinfo failed (port %s): %s\n", port, gai_strerror(rc));
            return -2;
        }

        /* Walk the list for one that we can bind to */
        for (p = listp; p; p = p->ai_next) {
            /* Create a socket descriptor */
            if ((socketfd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) 
                continue;  /* Socket failed, try the next */

            /* Eliminates "Address already in use" error from bind */
            setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR,    
                    (const void *)&optval , sizeof(int));

            /* Bind the descriptor to the address */
            if (bind(socketfd, p->ai_addr, p->ai_addrlen) == 0){
                printf("bind succeeded\n");
                break; /* Success */
            }
                
            if (close(socketfd) < 0) { /* Bind failed, try the next */
                fprintf(stderr, "open_listenfd close failed: %s\n", strerror(errno));
                return -1;
            }
        }

        freeaddrinfo(listp);

        if(!p){
            printf("open listen failed");
            return -1;
        }

        if(::listen(socketfd, LISTENQ)<0){
            printf("open listen failed\n");
            close(socketfd);
            return -1;
        }
        else{
            printf("listen succeeded\n");
            /* Regist event*/
            event_->setFD(socketfd);
            event_->enableReading();
        }
        return socketfd;
    }

    int ServerSocket::accept(){
        int connfd = ::accept(socketfd,(struct sockaddr*)&hostaddr,&hostlen);
        if(connfd<0) printf("accept failed\n");
        else{
            newConnectionFunc(connfd);
        }
        return connfd;
    }

    void ServerSocket::handleAccept(){
        int connfd = accept();
    }
}