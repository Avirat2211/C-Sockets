# C-Sockets
This is a simple TCP-based chat system implemented in C using POSIX sockets and multi-threading. It consists of a server that listens for incoming connections and relays messages between clients, and a client that connects to the server and exchanges messages.

## Features
- Supports multiple clients simultaneously.
- Clients can send messages to the server, which then broadcasts them to all connected clients.
- Messages are sent in the format: username: message.
- Threaded server handles multiple clients concurrently.
- Clients listen for incoming messages on a separate thread.

---

## Prerequisites
Ensure you have a c-compiler
 

## Running 
1. To start server, navigate to /server then
```bash
gcc main.c ../Socketutil/src/socketutil.c -o a.out
```
```bash
./a.out
```



2. To start clientside, navigate to /client then

```bash
gcc main.c ../Socketutil/src/socketutil.c -o a.out
```

```bash
./a.out
```


---

## Working
### Server
- Binds to a port and listens for incoming connections
- Assigns thread to each new client
- Relays recieved messages to all other connected clients

### Client
- Connects to server
- Username is requested
- Sends message to server which is then broadcasted
- Recieves message in a separate thread to allow simultaneous send/recieve


## Future Work
- Incorporate message encryption
- Support file transfers
- Better GUI for client
