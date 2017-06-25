## echo

echo is a simply example for illustrating socketx APIs. It consists of clients and a server. We describe their characteristics as follows.

- The client reads a line from the terminal input. Before sending this line to the server, it needs to connect to the server by socketx APIs:
```
 socketx::client_socket client;
 client.connect_to(host,port);
```
The client can simply communicate with the server by invoking `send_to` and `readline` member functions.