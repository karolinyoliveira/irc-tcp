#include "../lib/socket.hpp"
#include "../lib/utils.hpp"
#include "../lib/readline.hpp"

int main()
{
    clear_icanon();
    Socket s = Socket(8080);
    s.connect();
    int fileDescriptor = s.getfileDescriptor();

    string message;
    while (true)
    {
        cout << "> ";
        message = read_line_from_file(stdin);
        if(isCommand(message)){
            execCommand(message);
        }
            
        Socket::send(fileDescriptor, message);

        message = Socket::receive(fileDescriptor);
    }

    return 0;
}