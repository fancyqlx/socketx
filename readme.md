

**TODO: Refactor the library, the lib is undergoing radical changes...**

**TODO: We are going to complete the examples of new version of socketx and fix bugs :)**

# socketx

socketx is a library for network programming. It is writen by C/C++ and aims to provide C++ interfaces for users. This library is originally used for some simple projects of myself when I was learning networking programming. You can find more details in some of [Examples](##Examples).

## Features
socketx is based on I/O multiplexing and thread pool. It is a simplified reactor. There is one event demultiplexer for dispatching events, then the events are handled by corresponding callback functions. The library was designed for distributed computing projects, some features are designed for this purpose.

- Automatically dispatch events and handle errors
- C++ APIs for socket programming in Linux
- Provide thread pool
- Thread safe data structures
- Support robust writing and reading

## Examples
- [echo](./examples/echo/)
- [multi-echo](./examples/multi-echo/)
- [Minerx](https://github.com/fancyqlx/Minerx)
- [TinyHttpx](https://github.com/fancyqlx/TinyHttpx)

## New socketx is coming!
## QuickStart
In socketx, you just need to do at most four things:
- Think about what to do if a new connection comes.
- What do you want to write if a write events occur (This API is not necessary, you can invoke `socketx::Connection::send` everywhere).
- Find a container to save datas when a read events occrus and design your own program to handle these datas.
- Handle a connection close if you need. Socketx will take care of every connection and destory them in time.

To achive these targets, you need to complete four functions. We expose `socketx::Connection` to let you customize your own program to manage each connection. In this case, you can use the `send` and `write` API by `Connection`.
- NO.1 `void handleConection(std::shared_ptr<socketx::Connection> conn)`
- NO.2 `void handleWriteEvents(std::shared_ptr<socketx::Connection> conn)`
- NO.3 `void handleReadEvents(std::shared_ptr<socketx::Connection> conn)`
- NO.4 `void handleReadEvents(std::shared_ptr<socketx::Connection> conn)`

You can put them all into a class, then the rest of things will be handled by socketx. To let them work, you should regist them in a `Server` or a `Client`, depends on which class you used. It basically likes (Note that you should first create an EventLoop):

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

## Module
socketx has six modules for your programming. They are `socketx::EventLoop`, `socketx::Timer`, `socketx::Connection`, `socketx::Server`, `socketx::Client` and `socketx::ThreadPool`. For most of programs, `socketx::EventLoop`, `socketx::Connection`, `socketx::Server` and `socketx::Client` are required. For a simper program, `socketx::EventLoop` and `socketx::Connection` are enough. 

 **`socketx::EventLoop`** is the core of socketx. It utilizes `poll` to dispatch events, then invokes callback functions you registed. To regiest an event, you need `socketx::Connection` or `socketx::Timer`. `socketx::Connection` will manage a file descriptor and all events you interested. If a certain event occurs, you can handle this event by `socketx::Connection`, which encapsulates several sending and reading functions.

**`socketx::Timer`** is a much simpler module. It does not provide any functions for handle events it a timer is expiration. It just notifies you that it is the time, so you have to think about what you need to do if it occurs. A simple example for `socketx::Timer` is [multi-echo/EchoClient.cpp](./examples/multi-echo/EchoClient.cpp).

**`socketx::Server`** and **`socketx::Client`** are modules for creating server and client respectively. They encapsulate `bind`, `listen`, `accepte`, `connect` and so on. If you utilize these modules, you need provide hostname and port for them, then regist functions for events of new connections and read events. Like this:
```C++
server_->setHandleConnectionFunc(const std::function<void(std::shared_ptr<Connection>)> &func);
server_->setHandleReadEvents(const std::function<void(std::shared_ptr<Connection>)> &func);
```
If you want to handle something if the connection is closed, then regist a handleCloseEvents for it.

## The old version of socketx is [here](https://github.com/fancyqlx/socketx/tree/master/src_old_version)

