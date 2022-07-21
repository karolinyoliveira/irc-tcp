#include "../lib/utils.hpp"

#include <string>

bool isCommand(string str)
{
    return str[0] == '/';
}

void sigIntHandler(int sig_num)
{
    signal(SIGINT, sigIntHandler);
    cout << "Invalid command, if you want to leave please run /quit or CRTL+D :)" << endl;
    fflush(stdout);
}