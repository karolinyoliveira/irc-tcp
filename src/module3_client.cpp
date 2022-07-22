#include "../lib/socket.hpp"
#include "../lib/utils.hpp"
#include "../lib/readline.hpp"
#include <thread>
#include <mutex>

// Variáveis globais
int serverFD;
thread server_listener;
mutex mtx;


// Escuta o servidor
void listenFromServer(bool *can_exit) {
    string message;
    while ((*can_exit) == false) {
        mtx.lock();
        message = Socket::receive(serverFD);
        cout << message << endl;
        mtx.unlock();
    }
}


// Escuta o terminal do usuário
void listenFromTerminal(bool *can_exit, string nickname) {

    // String utilitária
    string message;

    // Execução enquanto não solicitar interrupção
    while ((*can_exit) == false) {

        // Processamento da mensagem
        message = read_line_from_file(stdin);
        if (isCommand(message) == false) {
            Socket::send(serverFD, nickname + message, 0);
        }

        // Processamento do comando
        else {
            switch(message[1]) {

                // connect
                case 'c': {
                    Socket::send(serverFD, nickname + "/connect", 0);
                    break;
                }
                
                // nickname
                case 'n': {
                    nickname = message.substr(message.find(' ') + 1, message.length() - 1);
                    nickname.append(": ");
                    break;
                }

                // quit
                case 'q': {
                    Socket::send(serverFD, nickname + "/quit", 0);
                    (*can_exit) = true;
                    break;
                }

                // comando sem correspondência: envia ao servidor
                default: {
                    Socket::send(serverFD, nickname + message, 0);
                    break;
                }
            }
        }
    }
}

int main()
{
    // Ignora ctrl + c e SIGPIPE
    signal(SIGINT, sigIntHandler);
    signal(SIGPIPE, SIG_IGN);

    // Socket
    Socket s = Socket(PORT);
    s.connect();
    serverFD = s.getfileDescriptor();

    // Controle de execução
    bool can_exit = false;

    // Apelido de usuário
    cout << "Enter a nickname: ";
    string nickname = read_line_from_file(stdin);
    nickname = nickname.substr(0, nickname.length() - 1);
    nickname.append(": ");

    // Threads
    thread terminal_listener = thread(listenFromTerminal, &can_exit, nickname);
    server_listener = thread(listenFromServer, &can_exit);

    // Inicialização da thread de terminal
    terminal_listener.join();
    server_listener.join();

    return 0;
}
