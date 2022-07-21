#include "../lib/socket.hpp"

#define NI_MAXHOST 1025
#define NI_MAXSERV 32

Socket::Socket(unsigned short port)
{
    // Socket creation
    // AF_INET for IPv4; SOCK_STREAM for TCP; 0 for IP
    this->fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);

    // Socket validation
    if (this->fileDescriptor == -1)
    {
        throw runtime_error("Failed to create TCP socket");
    }

    // Socket configuration
    int reuse = 1;
    if (setsockopt(this->fileDescriptor, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse) == -1)
    {
        throw runtime_error("Failed to define socket options");
    }

    // Socket address definition
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(port); // avoid endianness problems
}

void Socket::bind()
{
    if (::bind(this->fileDescriptor, (struct sockaddr *)&(this->address), sizeof(this->address)) == -1)
    {
        throw runtime_error("Failed to bind socket with the given address");
    }
}

Socket::~Socket()
{
    close(this->fileDescriptor);
}

void Socket::listen(int maxConnections)
{
    if (::listen(this->fileDescriptor, maxConnections) == -1)
    {
        throw runtime_error("Failed to listen to socket");
    }

    return;
}

int Socket::accept()
{
    int client;
    struct sockaddr_storage address;
    socklen_t addrlen = sizeof address;

    client = ::accept(this->fileDescriptor, (struct sockaddr *)&address, &addrlen);

    char host[NI_MAXHOST], port[NI_MAXSERV];

    getnameinfo((struct sockaddr *)&address, addrlen, host, sizeof(host), port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV);
    char msg[100];
    snprintf(msg, sizeof(char[100]), "Welcome user from ip %s, your code here is : %d!! \n", host, client);
    //aqui posso guardar os hosts num map se quiser :)
    ::send(client, msg, sizeof(char)*100, 0);

    return client;
}

void Socket::connect()
{
    if (::connect(this->fileDescriptor, (struct sockaddr *)&(this->address), sizeof(this->address)) == -1)
    {
        throw runtime_error("Failed to connect to socket");
    }

    return;
}

int Socket::send(int fileDescriptor, string msg)
{
    int nMessages = 0, bytes;

    while (msg.size())
    {
        string substr;

        if (msg.size() < MAX_MESSAGE_SIZE)
        {
            substr = msg;
            msg.clear();
        }
        else
        {
            substr = msg.substr(0, MAX_MESSAGE_SIZE);
            msg = msg.substr(MAX_MESSAGE_SIZE);
        }

        bytes = ::send(fileDescriptor, substr.c_str(), substr.size(), 0);
        nMessages++;

        if (bytes == -1)
        {
            throw runtime_error("Failed to send message");
        }
    }
    return nMessages;
}

string Socket::receive(int fileDescriptor)
{
    char buffer[MAX_MESSAGE_SIZE + 1];
    memset(buffer, 0, MAX_MESSAGE_SIZE);

    if (::recv(fileDescriptor, buffer, MAX_MESSAGE_SIZE, 0) < 0)
    {
        throw runtime_error("Failed to receive message");
    }
    return string(buffer);
}

int Socket::getfileDescriptor()
{
    return this->fileDescriptor;
}


string Socket::getAddress() 
{
    struct in_addr ipAddress = Socket::address.sin_addr;
    string output;
    inet_ntop( AF_INET, &ipAddress, output, INET_ADDRSTRLEN );
    return output;
}
