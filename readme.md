# socketx

socketx is a lib for wrapping linux APIs. It is writen by C/C++ and aims to provide C++ interfaces for users.

## Quickstart

### Client
#### Create a client
```C++
socketx::clientSocket client
```
#### Connect to a host
```C++
client.connect_to(host,port)
```

### Server
#### Create a server
```C++
socketx::serverSocket server
```
#### Listen to a port
```C++
server.listen_to(port)
```
#### Accept a connection from port
```C++
server.accept_from()
```

### Communication between hosts
#### Create a communication object
```C++
socketx::communication comm
```