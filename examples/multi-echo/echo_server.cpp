#include "socket.hpp"
#include "threadx.hpp"
#include <iostream>
#include <string>

socketx::cirqueue<std::string> data_queue(100);
std::vector<int> fdlist;

void echo_receive(int connfd){
    socketx::communication comm;
    comm.communication_init(connfd);
    char buf[MAXLINE];
    ssize_t n;
    while((n=comm.readline(buf,MAXLINE))>0){
        printf("server received %d bytes from client %d\n", (int)n, connfd);;
        data_queue.wait_push(std::string(buf));
    }
    comm.close_conn(connfd);
}

void echo_send(){
    std::string line;
    socketx::communication comm;
    while(1){
        data_queue.wait_pop(line);
        for(int i=0;i<fdlist.size();++i){
            comm.send(fdlist[i],line.c_str(),line.size());
        }
    }
}

int main(int argc,char** argv){

    if(argc!=2){
        fprintf(stderr,"usage: %s <port>\n", argv[0]);
        exit(0);
    }
    
    socketx::server_socket server;
    std::string port = argv[1];
    int listenfd = server.listen_to(port);

    socketx::thread_pool pool(5);
    

    pool.submit(echo_send);

    while(1){
        int connfd = server.accept_from();
        if(connfd>=0){
            std::string hostname = server.get_peername(connfd);
            size_t hostport = server.get_port();
            std::cout<<"connected to ("<<hostname<<", "<<hostport<<")"<<std::endl;
            pool.submit(std::bind(echo_receive,connfd));
            fdlist.push_back(connfd);
        }
    }
    exit(0);
}