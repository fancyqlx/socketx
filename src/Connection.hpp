#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "utilx.hpp"
#include "Event.hpp"
#include "Socket.hpp"

namespace socketx{

    /* Forward declaration*/
    /*Buffer*/
    class Buffer{
        public:
            Buffer(int fd):socketfd(fd),buffer(std::vector<char>(MAX_BUFSIZE))
            ,inputIt(buffer.begin()), outputIt(buffer.begin()){

            }

            ssize_t bufferWriter();
            ssize_t bufferReader(const std::string &data);
            size_t getFreesize(){
                return buffer.end()-inputIt;
            }

            size_t getDataSize(){
                return inputIt-outputIt;
            }

        private:
            std::vector<char> buffer;
            int socketfd;
            size_t tooidx;
            size_t toiidx;
            std::vector<char>::iterator inputIt;
            std::vector<char>::iterator outputIt;
            

    };



    class Connection: public Socket, public std::enable_shared_from_this<Connection>{
        public:
            Connection(EventLoop *loop, int fd);
            ~Connection();
            
            /*Regist events*/
            void registReadEvents(){
                event_->enableReading();
            }
            void registWriteEvents(){
                event_->enableWriting();
            }
            void unregistReadEvents(){
                event_->disableReading();
            }
            void unregistWriteEvents(){
                event_->disableWriting();
            }
            void unregist(){
                event_->deleteEvent();
            }

            /*Handle events*/
            void handleRead();
            void handleWrite();
            void handleError();
            void handleClose();

            /* Set user defined function*/
            void setHandleReadEvents(const std::function<void(std::shared_ptr<Connection>)> &func){
                handleReadEvents = func;
                readFun = true;
            }
            void setHandleWriteEvents(const std::function<void(std::shared_ptr<Connection>)> &func){
                handleWriteEvents = func;
                writeFun = true;
            }
            void setHandleCloseEvents(const std::function<void(std::shared_ptr<Connection>)> &func){
                handleCloseEvents = func;
                closeFun = true;
            }

            /*Send and write functions*/
            /*Send *n* bytes of buffer to the host it connected
            * Return the bytes it sends if succeed, return -1 otherwise.
            */
            ssize_t send(const void *buffer, size_t n);

            /*Receive bytes from the host it connected.
            * Save bytes to usrbuf with length n.
            * fd is initialized by communication_init(),
            * it means recv is connected to fd.
            */
            ssize_t recvFromBuffer(void *usrbuf, size_t n);
            std::string readline();
            /*Override function of recv.
            * This function needs a fd parameter,
            * It does not use the internal buffer.
            */
            ssize_t recv(void *usrbuf, size_t n);
            


            /*Send and receive messages*/
            ssize_t sendmsg(const Message &msg);
            Message recvmsgFromBuffer();
            Message recvmsg();

            ssize_t sendToBuffer(const std::string &data);
            ssize_t sendmsgToBuffer(const Message &msg);


        private:
            std::shared_ptr<Event> event_;
            EventLoop *loop_;

            bool readFun;
            bool writeFun;
            bool closeFun;
            std::function<void(std::shared_ptr<Connection>)> handleReadEvents;
            std::function<void(std::shared_ptr<Connection>)> handleWriteEvents;
            std::function<void(std::shared_ptr<Connection>)> handleCloseEvents;

            rio_t rio;
            std::shared_ptr<Buffer> buffer_;

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
    };


    



}


#endif