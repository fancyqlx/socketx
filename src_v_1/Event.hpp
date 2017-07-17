#ifndef EVENT_HPP
#define EVENT_HPP

namespace socketx{

    /*Forward declaration*/
    class EventLoop;

    class Event{
        public:
            Event(EventLoop *loop, int fd);

            void handleEvent();
            
            void setReadFunc(const std::function<void()> &func);
            void setWriteFunc(const std::function<void()> &func);
            void setErrorFunc(const std::function<void()> &func);

            void enableReading();
            void enableWriting();
        
        private:
            std::function<void()> readFunc;
            std::function<void()> writeFunc;
            std::function<void()> errorFunc;

            EventLoop *loop_;
            int fd_;
            int event_;
            int revent_;
    };


}
#endif