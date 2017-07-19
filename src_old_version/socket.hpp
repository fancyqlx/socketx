#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

#include "utilx.hpp"

namespace socketx{

    class socket{
        protected:
            struct addrinfo hints;
            struct sockaddr_storage hostaddr;
            socklen_t hostlen;
            /*Files descriptors*/
            int socketfd;
        public:
            /*Constructor*/
            socket();

            virtual ~socket();

            /*Return the hostname and port of the host it connect currently*/
            std::string get_hostname();
            std::string get_peername(int fd);
            size_t get_port();
            
            int close_conn(int fd);            
    };

    class message{
        private:
            size_t msize;
            char *data;
        public:
            message()=default;
            message(void * data_, size_t size_);
            message& operator=(const message &msg);
            size_t get_size() const;
            char * get_data() const;
    };

    class communication: public socket{
        private:
            rio_t rio;

            /*Connect the file descriptor to rio struct*/
            void rio_readinitb(int fd);

            /* 
            * rio_read - This is a wrapper for the Unix read() function that
            *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
            *    buffer, where n is the number of bytes requested by the user and
            *    rio_cnt is the number of unread bytes in the internal buffer. On
            *    entry, rio_read() refills the internal buffer via a call to
            *    read() if the internal buffer is empty.
            */
            ssize_t rio_read(char *usrbuf, size_t n);
            
        public:

            void communication_init(int fd);

            /*Send *n* bytes of buffer to the host it connected
            * Return the bytes it sends if succeed, return -1 otherwise.
            */
            ssize_t send(const int fd, const void *buffer, size_t n);

            /*Receive bytes from the host it connected.
            * Save bytes to usrbuf with length n.
            * fd is initialized by communication_init(),
            * it means recv is connected to fd.
            */
            ssize_t recv(void *usrbuf, size_t n);
            /*Override function of recv.
            * This function needs a fd parameter,
            * It does not use the internal buffer.
            */
            ssize_t recv(const int fd, void *usrbuf, size_t n);
            ssize_t readline(void *usrbuf, size_t n);


            /*Send and receive messages*/
            ssize_t sendmsg(const int fd, const message &msg);
            message recvmsg();
            message recvmsg(const int fd);
    };

    class server_socket: public communication{
        public:
            server_socket()=default;
            ~server_socket();

            /*Listen to a port*/
            int listen_to(const std::string port);

            /*Accept a connection.
            * Return a file descriptor.
            */
            int accept_from();
    };

    class client_socket: public communication{
        public:
            client_socket()=default;
            ~client_socket();
            /*Connect to a host*/
            int connect_to(const std::string hostname, const std::string port);
    };


    /*In unix, select will change the value of sets.
    * So we have to reset the set that needs to be selected.
    * Here we provide a wrapper for automatically managing 
    * fd_sets and maxfd. By using this wrapper, you need not 
    * reset the fd_set or compute the maxfd.
    */
    class select{
        private:
            std::bitset<1024> fd_bitset;
            void comp_maxfd();
        public:
            int maxfd;
            fd_set readset;
            fd_set writeset;
            fd_set exceptset;
            struct timeval timeout;

            select();

            int select_wrapper();
            void FD_zero(fd_set *fdset);
            void FD_set(int fd,fd_set *fdset);
            void FD_clr(int fd,fd_set *fdset);
            int FD_isset(int fd,fd_set *fdset);
    };

}

#endif