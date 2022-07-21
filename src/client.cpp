#include "../lib/socket.hpp"
#include "../lib/utils.hpp"
#include "../lib/readline.hpp"
#include <thread>
#include <mutex>

mutex mtx;
int serverFD;

void listenFromServer()
{
    string message, sender;
    while (true)
    {
        mtx.lock();
        message = Socket::receive(serverFD);
        sender = Socket::receive(serverFD);

        cout << sender << ": " << message << endl;
        mtx.unlock();
    }
}

void Connect()
{
    Socket::send(serverFD, "/connect");
    string message = Socket::receive(serverFD);
    cout << message << endl;
}

int main()
{
    signal(SIGINT, sigIntHandler);

    Socket s = Socket(PORT);
    s.connect();
    serverFD = s.getfileDescriptor();

    // --- Running client ---
    string message;

    // establishes connection with server
    Connect();
    thread t1(listenFromServer);

    while (true)
    {
        message = read_line_from_file(stdin);
        if (isCommand(message))
        {
            execCommand(message, serverFD);
        }

        Socket::send(serverFD, message);
    }

    return 0;
}
