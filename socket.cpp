#include "socket.hpp"

namespace socketx{
    /*Connect the file descriptor to rio struct*/
        void rio_readinitb(int fd){
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
        ssize_t rio_read(char *usrbuf, size_t n){
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

         /*Constructor*/
        socket(){
            clientfd = -1;
            listenfd = -1;
        }

        ~socket(){
            if(clientfd>0) close(clientfd);
            if(listenfd>0) close(listenfd);
        }
        
        /*Set hints for getaddrinfo function*/
        int set_protocol(struct addrinfo &hints);

        /*Create a client or a server*/
        int connect_to(const std::string hostname, const std::string port){
            struct addrinfo *listp, *p;
            char *hostname_ = hostname.c_str();
            char *port_ = port.c_str();


            memset(&hints, 0, sizeof(struct addrinfo));
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_flags = AI_NUMERICSERV;
            hints.ai_flags |= AI_ADDRCONFIG;
            getaddrinfo(hostname_, port_, &hints, &listp);

            for(p=listp;p;p=p->ai_next){
                if((clientfd=socket(p->ai_family,p->ai_socktype,p->ai_protocol))<0)
                    continue;
                
                if(connect(clientfd,p->ai_addr,p->ai_addrlen)!=-1)
                    break;
                close(clientfd);
            }

            freeaddrinfo(listp);
            if(!p){
                printf("connect failed\n");
                return -1;    
            }
            else{
                printf("connect succeeded\n");
                return clientfd;
            }
        }

        int listen_to(const std::string hostname="", const std::string port){
            struct addrinfo *listp, *p;
            int rc, optval=1;

            char *hostname_ = hostname.c_str();
            char *port_ = port.c_str();

            /* Get a list of potential server addresses */
            memset(&hints, 0, sizeof(struct addrinfo));
            hints.ai_socktype = SOCK_STREAM;             /* Accept connections */
            hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* ... on any IP address */
            hints.ai_flags |= AI_NUMERICSERV;            /* ... using port number */
            if ((rc = getaddrinfo(NULL, port_, &hints, &listp)) != 0) {
                fprintf(stderr, "getaddrinfo failed (port %s): %s\n", port, gai_strerror(rc));
                return -2;
            }

            /* Walk the list for one that we can bind to */
            for (p = listp; p; p = p->ai_next) {
                /* Create a socket descriptor */
                if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) 
                    continue;  /* Socket failed, try the next */

                /* Eliminates "Address already in use" error from bind */
                setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,    
                        (const void *)&optval , sizeof(int));

                /* Bind the descriptor to the address */
                if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0){
                    printf("bind succeeded\n");
                    break; /* Success */
                }
                    
                if (close(listenfd) < 0) { /* Bind failed, try the next */
                    fprintf(stderr, "open_listenfd close failed: %s\n", strerror(errno));
                    return -1;
                }
            }

            freeaddrinfo(listp);

            if(!p){
                printf("open listen failed");
                return -1;
            }

            if(listen(listenfd, LISTENQ)<0){
                printf("open listen failed");
                close(listenfd);
                return -1;
            }
            else{
                printf("listen succeeded\n");
            }

            return listenfd;
        }

        /*Send *n* bytes of buffer to the host it connected*/
        int send(const int fd, const void *buffer, size_t n){
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
        int recv(void *usrbuf, size_t n){
            size_t nleft = n;
            ssize_t nread;
            char *bufp = (char *)usrbuf;
            
            while (nleft > 0) {
                if ((nread = rio_read(rp, bufp, nleft)) < 0) 
                        return -1;          /* errno set by read() */ 
                else if (nread == 0)
                    break;              /* EOF */
                nleft -= nread;
                bufp += nread;
            }
            return (n - nleft);         /* return >= 0 */
        }
}