

**TODO: Refactor the library, the lib is undergoing radical changes...**

**TODO: We are going to complete the examples of new version of socketx and fix bugs :)**

# socketx

socketx is a library for wrapping linux APIs for socket programming. It is writen by C/C++ and aims to provide C++ interfaces for users. This library is originally used for some simple projects of myself when I was learning networking programming. You can find more details in some of [Examples](##Examples).

## Features
socketx is based on I/O multiplexing and thread pool. It is a simplified reactor. There is one event demultiplexer for dispatching events, then the events are handled by corresponding callback functions. The library was designed for distributed computing projects, so you may find that it is not applied for I/O intensive works.

- Automatically dispatch events and handle errors
- C++ APIs for socket programming in Linux
- Provide thread pool
- Thread safe data structures
- Support robust writing and reading

## New socketx is coming!
## QuickStart
In socketx, you just need to do three things:
- Think about what to do if a new connection comes
- What do you want to write if a write events occur.
- Find a container to save datas when a read events occrus and design your own program to handle these datas.
- Handle a connection close if you need. Socketx will take care of every connection and destory them in time.

To achive these targets, you need to complete three functions. We expose Connection to let you customize your own program to manage each connection. In this case, you can use the send and write API by connection.
- NO.1 `void handleConection(std::shared_ptr<Connection> conn)`
- NO.2 `void handleWriteEvents(std::shared_ptr<Connection> conn)`
- NO.3 `void handleReadEvents(std::shared_ptr<Connection> conn)`
- NO.4 `void handleReadEvents(std::shared_ptr<Connection> conn)`

You can put them all into a class, then the rest of the things will be handled by socketx. To let them work, you should regist them in a `Server` or a `Client`, depends on which class you used. It basically likes (Note that you should first create an EventLoop):

```C++
socketx::EventLoop loop;
socketx::Server server(&loop, port);
server.setHandleConnectionFunc(std::bind(handleConnection,std::placeholders::_1));
```
The last step is launching socketx. You need start server then let EventLoop run up.
```C++
server.start();
loop.loop();
```

## The old version of socketx is [here](https://github.com/fancyqlx/socketx/tree/master/src_old_version)

