#ifndef EVENTLOOP_HPP
#define EVENTLOOP_HPP

namespace socketx{

    class EventLoop{
        public:
            EventLoop() = default;
            ~EventLoop();

            void loop();

    };
}
#endif