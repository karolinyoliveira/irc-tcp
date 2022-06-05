#include "socket/socket.hpp"

int main()
{
    clear_icanon();
    Socket s = Socket("127.0.0.1", 3000);
    s.connect();
    int fileDescriptor = s.getfileDescriptor();

    string message;
    while (true)
    {
        cout << "> ";
        getline(cin, message);
        int messages = Socket::send(fileDescriptor, message);

        while (messages--)
            message = Socket::receive(fileDescriptor);
    }

    return 0;
}