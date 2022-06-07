#include "../lib/utils.hpp"
#include "../lib/readline.hpp"

int main() {

    clear_icanon();
    Socket s = Socket(8080);
    s.bind();
    s.listen(MAX_CONNECTIONS);

    int fileDescriptor = s.accept();

    string message;
    while (true) {

        message = Socket::receive(fileDescriptor);

        cout << "> ";
        /* ORIGINAL
        getline(cin, message);
        if (isCommand(message))
            execCommand(message);
        */
        message = read_line_from_file(stdin);
        if(isCommand(message)){
            execCommand(message);
        }

        Socket::send(fileDescriptor, message);
    }
    return 0;
}