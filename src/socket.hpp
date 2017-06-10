#ifndef _SOCKET_HPP_
#define _SOCKET_HPP_

namespace socketx{

class socket{
    private:
        struct rio_t rio;
        struct addrinfo hints;
        /*Files descriptors*/
        int clientfd;
        int listenfd;

        int host_type;

        /*Connect the file descriptor to rio struct*/
        void rio_readinitb(rio_t *rp, int fd);

        /* 
        * rio_read - This is a wrapper for the Unix read() function that
        *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
        *    buffer, where n is the number of bytes requested by the user and
        *    rio_cnt is the number of unread bytes in the internal buffer. On
        *    entry, rio_read() refills the internal buffer via a call to
        *    read() if the internal buffer is empty.
        */
        ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n);

    public:
        /*Constructor*/
        socket();

        ~socket();

        /*Set hints for getaddrinfo function*/
        int set_protocol(struct addrinfo &hints);

        /*Create a client or a server*/
        int connect_to(const std::string hostname, const std::string port);
        int listen_to(const std::string hostname="", const std::string port);

        /*Send *n* bytes of buffer to the host it connected*/
        int send(const int fd, const void *buffer, size_t n);

        /*Receive bytes from the host it connected.
        * Save bytes to usrbuf with length n.
        */
        int recv(const int fd, void *usrbuf, size_t n);
};


}

#endif