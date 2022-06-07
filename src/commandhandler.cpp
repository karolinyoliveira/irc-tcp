#include "../lib/utils.hpp"

#include <string>

bool isCommand(string str)
{
    return str[0] == '/';
}

void execCommand(string cmd)
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
        cout<<"pong"<<endl;
        break;
    }
    case 'q':
    {
        exit(0);
        break;
    }
    default:
    {
        cout<<"Unknown command"<<endl;
    }
    }
}