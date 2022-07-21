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

    // Descritor de arquivo do socket
    int file_descriptor;

public:
    
    /**
     * @brief Construtor para um novo objeto de usuário.
     * 
     * @param nickname apelido do usuário; limitado a 50 caracteres; validado pelo regex "^[A-Za-z0-9_]+$".
     * @param file_descriptor descritor de arquivo para o socket de conexão.
     * 
     * @throw std::invalid_argument caso o apelido informado ultrapasse 50 caracteres 
     * ou esteja mal-formatado, ou caso o file_descriptor informado seja <= 0.
     */
    User(string nickname, int file_descriptor);

    /// Operador de comparação.
    friend bool operator< (const User &left, const User &right);

    /// Obtém o apelido do usuário.
    string get_nickname();

    /// Retorna o file descriptor do socket de usuário.
    int get_file_descriptor();

};

#endif