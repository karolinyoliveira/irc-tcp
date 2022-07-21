#include "../lib/socket.hpp"
#include "../lib/utils.hpp"
#include "../lib/readline.hpp"
#include <thread>
#include <mutex>

mutex mtx;
int serverFD;

void listenFromServer()
{
    string message;
    while (true)
    {
        mtx.lock();
        message = Socket::receive(serverFD);
        cout << message << endl;
        mtx.unlock();
    }
}

void Connect()
{
    Socket::send(serverFD, "", 0);
    string message = Socket::receive(serverFD);
    cout << message << endl;
}

int main()
{
    signal(SIGINT, sigIntHandler);

    // --- Connecting client to server ---
    cout << "Start chatting with /connect" << endl;
    string message = read_line_from_file(stdin);

    if (!(message.compare("/connect")) || message.length() ==9)
    {
        Socket s = Socket(PORT);
        s.connect();
        serverFD = s.getfileDescriptor();

        // --- Running client ---

        Connect();

        thread t1(listenFromServer);

        while (true)
        {
            message = read_line_from_file(stdin);
            Socket::send(serverFD, message, 0);
            if (isCommand(message))
            {
                message = Socket::receive(serverFD);
                if (message == "bye")
                    return 0;
                cout << message << endl;
            }
        }

        return 0;
    }
    else
        exit(1);
}
