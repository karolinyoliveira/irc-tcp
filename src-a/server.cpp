#include "../lib/utils.hpp"

int main()
{
    clear_icanon();
    Socket s = Socket(8080);
    s.bind();
    s.listen(MAX_CONNECTIONS);

    int fileDescriptor = s.accept();

    string message;
    while (true)
    {
        message = Socket::receive(fileDescriptor);

        cout << "> ";
        getline(cin, message);
        if (isCommand(message))
            execCommand(message);
        Socket::send(fileDescriptor, message);
    }
    return 0;
}