#include "socket.hpp"

namespace socketx{

    /*********class socket***********/

        /*Constructor*/
    socket::socket(){
        hostlen = sizeof(struct sockaddr_storage);
        socketfd = -1;
    }

    socket::~socket(){
    }


    /*Return the hostname and port of the host it connect currently*/
    std::string socket::get_hostname(){
        struct sockaddr_in addr;
        socklen_t addr_len;
        getsockname(socketfd,(struct sockaddr *)&addr,&addr_len);
        return inet_ntoa(addr.sin_addr);
    }
    
    
    std::string socket::get_peername(int fd){
        struct sockaddr_in addr;
        socklen_t addr_len;
        getpeername(fd,(struct sockaddr *)&addr,&addr_len);
        return inet_ntoa(addr.sin_addr);
    }

    size_t socket::get_port(){
        struct sockaddr_in addr;
        socklen_t addr_len;
        getsockname(socketfd,(struct sockaddr *)&addr,&addr_len);
        return ntohs(addr.sin_port);
    }

    int socket::close_conn(int fd){
        return close(fd);
    }

    /**********class message************/
    message::message(void * data_, size_t size_){
        msize = size_;
        data = (char *)data_;
    }

    message& message::operator=(const message& msg){
        msize = msg.get_size();
        data = msg.get_data();
        return *this;
    }

    size_t message::get_size() const{
        return msize;
    }

    char * message::get_data() const{
        return data;
    }

    /*********class communication***********/

    /*Connect the file descriptor to rio struct*/
    void communication::rio_readinitb(int fd){
        rio.rio_fd = fd;  
        rio.rio_cnt = 0;  
        rio.rio_bufptr = rio.rio_buf;
    }

    /* 
    * rio_read - This is a wrapper for the Unix read() function that
    *    transfers min(n, rio_cnt) bytes from an internal buffer to a user
    *    buffer, where n is the number of bytes requested by the user and
    *    rio_cnt is the number of unread bytes in the internal buffer. On
    *    entry, rio_read() refills the internal buffer via a call to
    *    read() if the internal buffer is empty.
    */
    ssize_t communication::rio_read(char *usrbuf, size_t n){
        int cnt;

        while (rio.rio_cnt <= 0) {  /* Refill if buf is empty */
            rio.rio_cnt = read(rio.rio_fd, rio.rio_buf, 
                    sizeof(rio.rio_buf));
            if (rio.rio_cnt < 0) {
                if (errno != EINTR) /* Interrupted by sig handler return */
                return -1;
            }
            else if (rio.rio_cnt == 0)  /* EOF */
                return 0;
            else 
                rio.rio_bufptr = rio.rio_buf; /* Reset buffer ptr */
        }

        /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
        cnt = n;          
        if (rio.rio_cnt < n)   
            cnt = rio.rio_cnt;
        memcpy(usrbuf, rio.rio_bufptr, cnt);
        rio.rio_bufptr += cnt;
        rio.rio_cnt -= cnt;
        return cnt;
    }

    void communication::communication_init(int fd){
        rio_readinitb(fd);
    }


    /*Send *n* bytes of buffer to the host it connected*/
    ssize_t communication::send(const int fd, const void *buffer, size_t n){
        size_t nleft = n;
        ssize_t nwritten;
        char *bufp = (char *)buffer;

        while (nleft > 0) {
            if ((nwritten = write(fd, bufp, nleft)) <= 0) {
                if (errno == EINTR)  /* Interrupted by sig handler return */
                    nwritten = 0;    /* and call write() again */
                else
                    return -1;       /* errno set by write() */
            }
            nleft -= nwritten;
            bufp += nwritten;
        }
        return n;
    }

    /*Receive bytes from the host it connected.
    * Save bytes to usrbuf with length n.
    */
    ssize_t communication::recv(void *usrbuf, size_t n){
        size_t nleft = n;
        ssize_t nread;
        char *bufp = (char *)usrbuf;
        
        while (nleft > 0) {
            if ((nread = rio_read(bufp, nleft)) < 0) 
                    return -1;          /* errno set by read() */ 
            else if (nread == 0)
                break;              /* EOF */
            nleft -= nread;
            bufp += nread;
        }
        return (n - nleft);         /* return >= 0 */
    }

    ssize_t communication::readline(void *usrbuf, size_t maxlen){
        int n, rc;
        char c, *bufp = (char *)usrbuf;

        for (n = 1; n <= maxlen; n++) { 
            if ((rc = rio_read(&c, 1)) == 1) {
                *bufp++ = c;
                if (c == '\n') {
                    n++;
                    break;
                }
            } else if (rc == 0) {
                if (n == 1)
                    return 0; /* EOF, no data read */
                else
                    break;    /* EOF, some data was read */
            } else
            return -1;	  /* Error */
        }
        *bufp = 0;
        return n-1;
    }

    /*Send and receive messages*/
    ssize_t communication::sendmsg(const int fd, const message &msg){
        size_t n = msg.get_size();
        char * buffer = msg.get_data();
        /*Send the size of the message first*/
        if(send(fd,&n,sizeof(n))>0){
            /*Send data*/
            if(send(fd,buffer,n)>0)
                return 1;
        }
        return -1;
    }

    message communication::recvmsg(){
        size_t n = 0;
        /*Receive the size of the message*/
        if(recv(&n,sizeof(size_t))>0){
            /*Recieve the message*/
            char * data = new char[n];
            recv(data,n);
            return message(data,n);
        }
        else return message(nullptr,0);
    }

    /*********class server_socket***********/

    server_socket::~server_socket(){
    }


    int server_socket::listen_to(const std::string port){
        struct addrinfo *listp, *p;
        int rc, optval=1;

        const char *port_ = port.c_str();

        /* Get a list of potential server addresses */
        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_socktype = SOCK_STREAM;             /* Accept connections */
        hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* ... on any IP address */
        hints.ai_flags |= AI_NUMERICSERV;            /* ... using port number */
        if ((rc = getaddrinfo(NULL, port_, &hints, &listp)) != 0) {
            fprintf(stderr, "getaddrinfo failed (port %s): %s\n", port_, gai_strerror(rc));
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

        if(listen(socketfd, LISTENQ)<0){
            printf("open listen failed\n");
            close(socketfd);
            return -1;
        }
        else{
            printf("listen succeeded\n");
        }
        return socketfd;
    }

    /*Accept a connection.
    * Return a file descriptor.
    */
    int server_socket::accept_from(){
        int connfd = accept(socketfd,(struct sockaddr*)&hostaddr,&hostlen);
        if(connfd<0) printf("accept failed\n");
        return connfd;
    }

    /*********class client_socket***********/

    /*Create a client or a server*/

    client_socket::~client_socket(){
    }

    int client_socket::connect_to(const std::string hostname, const std::string port){
        struct addrinfo *listp, *p;
        const char *hostname_ = hostname.c_str();
        const char *port_ = port.c_str();


        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_NUMERICSERV;
        hints.ai_flags |= AI_ADDRCONFIG;
        getaddrinfo(hostname_, port_, &hints, &listp);

        for(p=listp;p;p=p->ai_next){
            if((socketfd=::socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0)
                continue;
            
            if(connect(socketfd,p->ai_addr,p->ai_addrlen)!=-1)
                break;
            close(socketfd);
        }

        freeaddrinfo(listp);
        if(!p){
            printf("connect failed\n");
            return -1;    
        }
        else{
            printf("connect succeeded\n");
            return socketfd;
        }
    }

    /****************class select*********************/
    
    void select::comp_maxfd(){
        int n = fd_bitset.size();
        for(int i=0;i<n;++i){
            if(fd_bitset[i]==1 && i>maxfd)
                maxfd = i;
        }
    }

    select::select():maxfd(0){
        FD_zero(&readset);
        FD_zero(&writeset);
        FD_zero(&exceptset);
    }

    int select::select_wrapper(){
        return ::select(maxfd+1,&readset,&writeset,&exceptset,&timeout);
    }

    void select::FD_zero(fd_set *fdset){
        FD_ZERO(fdset);
        fd_bitset.reset();
        maxfd = 0;
    }

    void select::FD_set(int fd,fd_set *fdset){
        FD_SET(fd,fdset);
        fd_bitset[fd] = 1;
        if(fd>maxfd) maxfd = fd;
    }

    void select::FD_clr(int fd,fd_set *fdset){
        FD_CLR(fd,fdset);
        fd_bitset[fd] = 0;
        if(fd==maxfd) comp_maxfd();
    }

    int select::FD_isset(int fd,fd_set *fdset){
        return FD_ISSET(fd,fdset);
    }

    
}
