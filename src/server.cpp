#include "../lib/utils.hpp"
#include "../lib/readline.hpp"

int main()
{
    Socket s = Socket(PORT);
    s.bind();
    s.listen(MAX_CONNECTIONS);

    int fileDescriptor = s.accept();

    string message;
    while (true)
    {

        message = Socket::receive(fileDescriptor);

        if (isCommand(message))
        {
            execCommand(message, fileDescriptor);
        }
        else
        {
            message = read_line_from_file(stdin);
            Socket::send(fileDescriptor, message);
        }
    }
    return 0;
}