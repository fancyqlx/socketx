# socketx

socketx is a lib for wrapping linux APIs for socket programming. It is writen by C/C++ and aims to provide C++ interfaces for users.

## Features
- c++ APIs for socket programming in Linux
- Provide thread pool
- Thread safe data structures
- Support robust writing and reading

## Quickstart

### Client
- Create a client
```C++
socketx::client_socket client
```
- Connect to a host
```C++
client.connect_to(host,port)
```

### Server
- Create a server
```C++
socketx::server_socket server
```
- Listen to a port
```C++
server.listen_to(port)
```
- Accept a connection from port
```C++
server.accept_from()
```

### Communication between hosts
- Create a communication object
```C++
socketx::communication comm
```

### Thread pool
- Create a thread pool
```C++
socketx::thread_pool pool;
socketx::thread_pool pool(4);
```

- Submit a task
```C++
pool.submit(std::bind(task,args));
```