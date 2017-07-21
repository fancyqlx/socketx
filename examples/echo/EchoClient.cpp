#include "EventLoop.hpp"
#include "Client.hpp"
#include "Connection.hpp"

class Client_test{
    public:
        Client_test(socketx::EventLoop *loop, std::string hostname, std::string port)
        :loop_(loop), hostname_(hostname),port_(port),
        client_(new socketx::Client(loop,hostname,port)){
            client_->setHandleConnectionFunc(std::bind(&Client_test::handleConnection, this, std::placeholders::_1));
            client_->setHandleCloseEvents(std::bind(&Client_test::handleCloseEvents, this, std::placeholders::_1));
            /*Get file descriptor of stdin and regist it into EventLoop*/
            int fd = fileno(stdin);
            stdinConn = std::make_shared<Connection>(loop_,fd);
            stdinConn->setHandleReadEvents(std::bind(&Client_test::stdinReadEvents, this, std::placeholders::_1))
            stdinConn->registReadEvents();
        }

        void start(){
            client_->start();
        }

        void stdinReadEvents(std::shared_ptr<socketx::Connection> conn){
            std::string line;
            if(std::getline(std::cin,line)){
                line += '\n';
                clientConn->send(line.c_str(),line.size());
            }
            else(
                printf("Read error from stdin....\n");
            )
        }

        void handleConnection(std::shared_ptr<socketx::Connection> conn){
            printf("New connection comes, we are going to set read events!!!\n");
            client_->setHandleReadEvents(std::bind(&Client_test::handleReadEvents, this, std::placeholders::_1));
            clientConn = conn;
        }
        void handleReadEvents(std::shared_ptr<socketx::Connection> conn){
            std::string line = conn->readline();
            if(line.size()==0) conn_->handleClose();
            else std::cout<<line<<endl;
        }
        void handleCloseEvents(std::shared_ptr<socketx::Connection> conn){
            printf("Close connection...\n");
        }

    private:
        std::shared_ptr<Connection> stdinConn;
        std::shared_ptr<Connection> clientConn;
        socketx::EventLoop *loop_;
        socketx::Client *client_;
        std::string hostname_;
        std::string port_;
};


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