#include "../lib/utils.hpp"
#include "../lib/readline.hpp"
#include <vector>

#define MAX_CLIENTS 10
vector<int> clients(MAX_CLIENTS);

void closeConnection(int fd)
{
    Socket::send(fd, "bye", 0);
    close(fd);
}

int main()
{
    // --- Starting server ---
    Socket s = Socket(PORT);
    s.bind();
    s.listen(MAX_CONNECTIONS);
    int serverFD = s.getfileDescriptor();

    // --- Running server ---
    fd_set fdset;
    int maxFD, currFD, ready, err;
    string message;

    while (true)
    {
        FD_ZERO(&fdset);

        FD_SET(serverFD, &fdset);
        maxFD = serverFD;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            // verifies if there's a valid client in i and stores it in fdset
            currFD = clients[i];
            if (currFD)
                FD_SET(currFD, &fdset);

            // save the max value of fd in fdset to use select()
            if (currFD > maxFD)
                maxFD = currFD;
        }

        // wait indefinitely for activity on any socket (timeout is NULL)
        ready = select(maxFD + 1, &fdset, NULL, NULL, NULL);
        if ((ready < 0) && (errno != EINTR))
        {
            printf("Failed to retrieve the number of ready descriptors");
        }

        // if the socket of the server is ready, there's a connection attempt
        if (FD_ISSET(serverFD, &fdset))
        {
            currFD = s.accept();

            // add new socket to clients[]
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (clients[i] == 0)
                {
                    clients[i] = currFD;
                    break;
                }
            }
        }

        // observing the clients
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            currFD = clients[i];

            if (FD_ISSET(currFD, &fdset))
            {
                int pongFlag = 0;
                message = Socket::receive(currFD);

                if (isCommand(message))
                {
                    switch (message[1])
                    {
                    case 'p': // ping
                    {
                        err = Socket::send(currFD, "pong", 0);
                        if (err == -1)
                        {
                            closeConnection(clients[i]);
                            clients[i] = 0;
                        }
                        pongFlag++;
                        break;
                    }
                    case 'q': // quit
                    {
                        closeConnection(currFD);
                        clients[i] = 0;
                        break;
                    }
                    default:
                    {
                        err = Socket::send(currFD, "invalid command", 0);
                        if (err == -1)
                        {
                            closeConnection(clients[i]);
                            clients[i] = 0;
                        }
                    }
                    }
                }
                else if (!pongFlag)
                {
                    for (int i = 0; i < MAX_CLIENTS; i++)
                    {
                        string fmtMessage = to_string(currFD) + ": " + message;
                        if (clients[i] != 0 && clients[i] != currFD)
                        {
                            err = Socket::send(clients[i], fmtMessage, 0);
                            if (err == -1)
                            {
                                closeConnection(clients[i]);
                                clients[i] = 0;
                            }
                        }
                    }
                }
                pongFlag = 0;
            }
        }
    }
    return 0;
}
