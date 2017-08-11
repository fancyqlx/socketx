#ifndef UTILX_HPP
#define UTILX_HPP

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <poll.h>
#include <sys/stropts.h>
#include <sys/timerfd.h>
#include <time.h>

#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <queue>
#include <bitset>
#include <algorithm>
#include <map>

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <stdexcept>
#include <chrono>

#include <random>


namespace socketx{
    /* $begin rio_t */
    #define RIO_BUFSIZE 8192
    #define MAXLINE 8192
    #define LISTENQ 1024

    const int INFTIM = -1;

    typedef struct {
        int rio_fd;                /* Descriptor for this internal buf */
        int rio_cnt;               /* Unread bytes in internal buf */
        char *rio_bufptr;          /* Next unread byte in internal buf */
        char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
    } rio_t;
    /* $end rio_t */

    /*Message for sending formative datas*/
    class Message{
        private:
            size_t msize;
            char *data;

        public:
            Message()=default;

            ~Message(){
                delete data;
            }

            Message(void * data_, size_t size_){
                msize = size_;
                data = (char *)data_;
            }

            Message(const Message &msg){
                this->msize = msg.getSize();
                this->data = new char[this->msize];                   
                memcpy(this->data,msg.getData(),msize);
            }

            Message& operator=(const Message &msg){
                if(this!=&msg){
                    delete this->data;
                    msize = msg.getSize();
                    this->data = new char[this->msize];                   
                    memcpy(this->data,msg.getData(),msize);
                }
                return *this;
            }

            size_t getSize() const{
                return msize;
            }

            char * getData() const{
                return data;
            }
    };

    class IgnoreSIGPIPE{
        public:
            IgnoreSIGPIPE(){
                ::signal(SIGPIPE,SIG_IGN);
            }
    };

}

#endif