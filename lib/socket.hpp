#ifndef socket_HPP
#define socket_HPP

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/time.h>

#define MAX_MESSAGE_SIZE 4096
#define MAX_CONNECTIONS 10
#define PORT 8080

using namespace std;

class Socket
{
private:
    int fileDescriptor; // remove
    struct sockaddr_in address;

public:
    // Initializes a client socket with specific port and address INADDR_ANY
    Socket(unsigned short port);

    // Closes the file fileDescriptor
    ~Socket();

    // Sends messages with max size of 4096 chars or multiple small messages
    static int send(int fd, string msg);

    // Receives a message and prints it to stdout. [Format:"sender": "message"]
    static string receive(int fd);

    // Binds ip address and port to a socket
    void bind();

    // Starts listening
    void listen(int maxConnections);

    // Accepts a connection from a client, returns its fileDescriptor or -1
    int accept();

    // Connects client to a server
    void connect();

    // Gets the file fileDescriptor
    int getfileDescriptor();

    // Gets the IPv4 address
    string getAddress();
};

#endif