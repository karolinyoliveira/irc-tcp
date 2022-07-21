// Para controle de pré-compilação
#ifndef CHANNEL_CONTROLLER_HPP
#define CHANNEL_CONTROLLER_HPP

// Dependências
#include "./socket.hpp"
#include "./channel.hpp"
#include "./user.hpp"
#include <map>
#include <thread>
using namespace std;

/**
 * @brief Controlador de canais de comunicação RFC-1459.
 */
class ChannelController {
private:
    
    // Canais
    map<string, Channel *> channels;

    // Controle de conexões
    int max_connections;
    fd_set fdset;
    /*
    chave: file_descriptor do usuário
    valor: par <nome do canal ao qual está conectado, usuário>
    */
    map<int, pair<string *, User *>> connections;

    // Socket para comunicação
    Socket *server_socket = NULL;

    // Controle de threads
    bool may_exit = false;

    // Threads
    thread connections_thread;
    thread messages_thread;

    // Lógica das threads
    void connections_thread_logic();
    bool verify_credentials (
        map<int, pair<string *, User *>>::iterator connection_itr, 
        map<string, Channel *>::iterator *channel_itr, 
        bool check_admin, 
        int currFD
    );
    void messages_thread_logic();

public:
    
    /**
     * @brief Construtor para um novo objeto controlador de canais.
     * @param max_connections quantia global máxima de usuários.
     * @throws std::invalid_argument caso a quantia de conexões máximas fornecida seja inválida (<= 0).
     */
    ChannelController(int max_connections);

    /**
     * @brief Conecta um usuário ao canal especificado. Caso não exista tal 
     * canal previamente, um novo é criado com base no nome informado.
     * 
     * @param user ponteiro para o objeto de usuário de canal.
     * @param channel_name nome do canal ao qual conectar.
     * @param need_invitation indica se o canal precisa ou não de convite.
     * @return true caso a conexão seja bem-sucedida.
     * @return false caso o canal solicitado seja inexistente.
     * @throws std::invalid_argument caso seja informado um ponteiro nulo.
     */
    bool join_channel(User *user, string channel_name, bool need_invitation);

    /// Inicializa o controlador.
    void start();

    /// Encerra o controlador.
    void stop();
};

#endif