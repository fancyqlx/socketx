#include "socket.hpp"
#include "threadx.hpp"
#include <iostream>
#include <string>


void echo_client(int fd){
    socketx::communication comm;
    comm.communication_init(fd);
    std::string line;
    char * buf = new char[MAXLINE];

    /*Generate a random number*/
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1,10);

    while(1){
        /*Sleep for a random time before sending messages*/
        auto x = uni(rng);
        std::this_thread::sleep_for(std::chrono::seconds(x));

        line = "message from client " + std::to_string(fd) + "\n";
        comm.send(fd,line.c_str(),line.size());
        comm.readline(buf,MAXLINE);
        fputs(buf,stdout);
    }
    comm.close_conn(fd);
}


int main(int argc, char **argv){
    
    if(argc!=3) {
        std::cout<<"usage: "<<argv[0]<<"<host> "<<"<port> "<<std::endl;
        exit(0);
    }
    std::string host(argv[1]);
    std::string port(argv[2]);

    socketx::thread_pool pool(5);
    socketx::client_socket client;

    for(int i=0;i<5;++i){
        int clientfd = client.connect_to(host,port);
        pool.submit(std::bind(echo_client,clientfd));
    }
    return 0; 
}