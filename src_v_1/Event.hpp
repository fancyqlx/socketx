#ifndef EVENT_HPP
#define EVENT_HPP

namespace socketx{

    /*Forward declaration*/
    class EventLoop;

    class Event{
        public:
            Event(EventLoop *loop, int fd);


            
            void handleEvent();
            
            

            void enableReading();
            void enableWriting();
            
            /*Set callback function*/
            void setReadFunc(const std::function<void()> &func){
                readFunc = func;
            }
            void setWriteFunc(const std::function<void()> &func){
                writeFunc = func;
            }
            void setErrorFunc(const std::function<void()> &func){
                errorFunc = func;
            }

            /*Get or set some data member */
            int getFD(){
                return fd_;
            }
            void setEvents(int events){
                events_ = events;
            }
            void setRevents(int revents){
                revents_ = revents;
            }
        
        private:
            std::function<void()> readFunc;
            std::function<void()> writeFunc;
            std::function<void()> errorFunc;

            EventLoop *loop_;
            int fd_;
            int events_;
            int revents_;
    };


}
#endif