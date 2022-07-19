// Para controle de pré-compilação
#ifndef USER_HPP
#define USER_HPP

// Dependências
#include <string>
using namespace std;

/**
 * @brief Implementa um usuário para o serviço de comunicação.
 */
class User {
private:
    
    // Apelido do usuário
    string nickname;

public:
    
    /**
     * @brief Construtor para um novo objeto de usuário.
     * @param nickname apelido do usuário.
     */
    User(string nickname);

    /// Operador de comparação.
    friend bool operator< (const User &left, const User &right);

    /// Obtém o apelido do usuário.
    string get_nickname();

    /// Define um novo apelido de usuário.
    void set_nickname(string nickname);

};

#endif