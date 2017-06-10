#ifndef UTILX_HPP
#define UTILX_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include <iostream>

namespace socketx{
    /* $begin rio_t */
    #define RIO_BUFSIZE 8192
    #define SERVER 0
    #define CLIENT 1

    typedef struct {
        int rio_fd;                /* Descriptor for this internal buf */
        int rio_cnt;               /* Unread bytes in internal buf */
        char *rio_bufptr;          /* Next unread byte in internal buf */
        char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
    } rio_t;
    /* $end rio_t */
}

#endif