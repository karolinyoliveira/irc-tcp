#include "../lib/utils.hpp"
#include "../lib/readline.hpp"
#include <vector>

#define MAX_CLIENTS 10

int main()
{

    // --- Starting server ---
    Socket s = Socket(PORT);
    s.bind();
    s.listen(MAX_CONNECTIONS);

    // --- Running server ---
    vector<int> clients(MAX_CLIENTS);
    fd_set fdset;
    int maxFD, currFD, ready;
    string message;

    while (true)
    {
        FD_ZERO(&fdset);

        FD_SET(s.getfileDescriptor(), &fdset);
        maxFD = s.getfileDescriptor();

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
        if (FD_ISSET(s.getfileDescriptor(), &fdset))
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

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            currFD = clients[i];

            if (FD_ISSET(currFD, &fdset))
            {

                message = Socket::receive(currFD);

                if (isCommand(message))
                {
                    execCommand(message, s.getfileDescriptor()); // talvez seja melhor colocar o switch aqui msm (facilita pra fechar um socket pelo menos)
                }
                else
                {
                    for (int i = 0; i < MAX_CLIENTS; i++)
                    {
                        if (clients[i] != 0 && clients[i] != currFD)
                            Socket::send(clients[i], message);
                    }
                }
            }
        }
    }
    return 0;
}