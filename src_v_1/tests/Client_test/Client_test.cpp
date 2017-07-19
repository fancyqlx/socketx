#include "EventLoop.hpp"
#include "Client.hpp"

class Client_test{
    public:
        Client_test(socketx::EventLoop *loop, std::string hostname, std::string port)
        :loop_(loop), hostname_(hostname),port_(port),
        client_(new Client(loop,hostname,port)){
            client_->setHandleConnectionFunc(std::bind(&Client_test::handleConnection,this));
        }

        void start(){
            server_->start();
        }

        void handleConnection(){
            printf("New connection comes, we are going to set read events!!!\n");
            server_->setHandleReadEvents(std::bind(&Server_test::handleReadEvents,this));
        }
        void handleReadEvents(){
            printf("Read events....\n");
        }

    private:
        socketx::Eventloop *loop_;
        socketx::Client *client_;
        std::string hostname_;
        std::string port_;
}


int main(int argc, char **argv){
    if(argc!=3){
        fprintf(stderr,"usage: %s <port>\n", argv[0]);
        exit(0);
    }
    std::string hostname(argv[1]);
    std::string port(argv[2]);
    socketx::EventLoop loop;
    Client_test client(&loop,hostname,port);
    client.start();
    loop.loop();

    return 0;
}