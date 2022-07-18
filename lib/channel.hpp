// Para controle de pré-compilação
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

// Dependências
#include "./user.hpp"
#include <string>
#include <set>
using namespace std;

/**
 * @brief Implementa um canal segundo as restrições do protocolo RFC-1459, 
 * ainda que simplifique alguns aspectos. Para tanto, baseia-se em comunicação 
 * via sockets.
 */
class Channel {
private:
    
    // Nome do canal
    string name;

    // Usuário administrador
    User *admin = NULL;

    // Demais usuários
    set<User *> users;

public:
    
    /**
     * @brief Construtor para um novo objeto de canal.
     * @param name nome do canal segundo as restrições RFC-1459.
     * @throws std::invalid_argument caso o nome não siga a restrições RFC-1459.
     */
    Channel(string name);

    /// Operador de comparação.
    friend bool operator< (const Channel &left, const Channel &right);

    /** 
     * Pedido de junção ao canal.
     * @throws std::invalid_argument caso seja informado um ponteiro nulo.
     */
    void join(User *user);
};

#endif