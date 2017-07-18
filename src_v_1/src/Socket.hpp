#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include "utilx.hpp"

namespace socketx{

    class Socket{
        protected:
            struct addrinfo hints;
            struct sockaddr_storage hostaddr;
            socklen_t hostlen;
            /*Files descriptors*/
            int socketfd;
        public:
            /*Constructor*/
            Socket();

            virtual ~Socket();

            /*Return the hostname and port of the host it connect currently*/
            std::string get_hostname();
            std::string get_peername(int fd);
            size_t get_port();
            
            int closeConn();            
    };

    class ServerSocket{
        public:
            ServerSocket()=default;
            ~ServerSocket();

            /*Listen to a port*/
            int listen();

            /*Accept a connection.
            * Return a file descriptor.
            */
            int accept();
    };

    class ClientSocket{
        public:
            ClientSocket()=default;
            ~ClientSocket();
            /*Connect to a host*/
            int connect();
    };
}

#endif