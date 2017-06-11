#include "socket.hpp"
#include <iostream>
#include <string>

int main(int argc, char **argv){
    socketx::clientSocket client;
    if(argc!=3) {
        std::cout<<"usage: "<<argv[0]<<"<host> "<<"<port> "<<std::endl;
        exit(0);
    }
    std::string host(argv[1]);
    std::string port(argv[2]);
    int clientfd = client.connect_to(host,port);
    client.communication_init(clientfd);

    std::string line;
    while(clientfd>=0 && std::getline(std::cin,line)){
        line.push_back('\n');
        size_t size = line.size();
        client.send(clientfd,line.c_str(),size);
        char * buf = new char[size+1];
        client.readline(buf,size);
        fputs(buf,stdout);
    }
    close(clientfd);
    exit(0); 
}