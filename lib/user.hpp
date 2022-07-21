// Para controle de pré-compilação
#ifndef USER_HPP
#define USER_HPP

// Dependências
#include <string>
#include "./socket.hpp"
using namespace std;

/**
 * @brief Implementa um usuário para o serviço de comunicação.
 */
class User {
private:
    
    // Apelido do usuário
    string nickname;

    // Socket do usuário
    Socket *socket = NULL;

public:
    
    /**
     * @brief Construtor para um novo objeto de usuário.
     * 
     * @param nickname apelido do usuário; limitado a 50 caracteres.
     * @param socket ponteiro para um socket a ser utilizado para conexão.
     * 
     * @throw std::invalid_argument caso o apelido informado ultrapasse 50 caracteres.
     * @throw std::invalid_argument caso o ponteiro informado seja nulo.
     */
    User(string nickname, Socket *socket);

    /// Operador de comparação.
    friend bool operator< (const User &left, const User &right);

    /// Obtém o apelido do usuário.
    string get_nickname();

    /// Retorna o file descriptor do socket de usuário.
    int get_file_descriptor();

};

#endif