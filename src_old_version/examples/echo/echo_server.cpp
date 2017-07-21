#include "socket.hpp"
#include "threadx.hpp"
#include <iostream>
#include <string>

void echo(int connfd){
    socketx::communication comm;
    comm.communication_init(connfd);
    char buf[MAXLINE];
    ssize_t n;
    while((n=comm.readline(buf,MAXLINE))>0){
        printf("server received %d bytes from child server %d\n", (int)n, (int)getpid());
        comm.send(connfd,buf,n);
        fputs(buf,stdout);
    }
    comm.close_conn(connfd);
}

int main(int argc,char** argv){

    if(argc!=2){
        fprintf(stderr,"usage: %s <port>\n", argv[0]);
        exit(0);
    }
    
    socketx::server_socket server;
    std::string port = argv[1];
    int listenfd = server.listen_to(port);

    socketx::thread_pool pool;

    while(1){
        int connfd = server.accept_from();
        if(connfd>=0){
            std::string hostname = server.get_peername(connfd);
            size_t hostport = server.get_port();
            std::cout<<"connected to ("<<hostname<<", "<<hostport<<")"<<std::endl;
            pool.submit(std::bind(echo,connfd));
        }
    }
    exit(0);
}