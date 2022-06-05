#include "socket/socket.hpp"

int main()
{
    clear_icanon();
    Socket s = Socket("127.0.0.1", 3000);
    s.bind();
    s.listen(MAX_CONNECTIONS);

    int fileDescriptor = s.accept();

    string message;
    while (true)
    {
        message = Socket::receive(fileDescriptor);

        cout<<"> ";
        getline(cin,message);
        Socket::send(fileDescriptor, message);
    }
    return 0;
}