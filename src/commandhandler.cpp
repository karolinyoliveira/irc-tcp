#include "../lib/utils.hpp"

#include <string>

bool isCommand(string str)
{
    return str[0] == '/';
}

void execCommand(string cmd, int fileDescriptor)
{
    switch (cmd[1])
    {
    case 'c':
    {
        // connects a client to the server
        break;
    }
    case 'p':
    {
        Socket::send(fileDescriptor, "pong");
        break;
    }
    case 'q':
    {
        exit(0);
        break;
    }
    default:
    {
        cout << "Invalid command" << endl;
    }
    }
}

void sigIntHandler(int sig_num)
{
    signal(SIGINT, sigIntHandler);
    cout << "Invalid command, if you want to leave please run /quit or CRTL+D :)" << endl;
    fflush(stdout);
}