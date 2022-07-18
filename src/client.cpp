#include "../lib/socket.hpp"
#include "../lib/utils.hpp"
#include "../lib/readline.hpp"

int main()
{
    signal(SIGINT, sigIntHandler);
    
    Socket s = Socket(PORT);
    s.connect();
    int fileDescriptor = s.getfileDescriptor();

    string message;
    while (true)
    {
        message = read_line_from_file(stdin);
        if (isCommand(message))
        {
            execCommand(message, fileDescriptor);
        }

        Socket::send(fileDescriptor, message);

        message = Socket::receive(fileDescriptor);
    }

    return 0;
}