// Para controle de pré-compilação
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

// Dependências
#include "./user.hpp"
#include <string>
#include <map>
using namespace std;

/**
 * @brief Implementa um canal segundo as restrições do protocolo RFC-1459, 
 * ainda que simplifique alguns aspectos. Para tanto, baseia-se em comunicação 
 * via sockets.
 */
class Channel {
private:

    /**
     * @brief Classe interna para um usuário de canal.
     */
    class ChannelUser {
    private:

        // Atributos
        User *user = NULL;
        bool muted = false;

    public:

        // Contrutor
        ChannelUser(User *user);

        // Setters
        void mute();
        void unmute();

    };
    
    // Nome do canal
    string name;

    // Usuário administrador
    User *admin = NULL;

    // Demais usuários
    map<string, ChannelUser *> users;

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
     * @returns verdadeiro caso seja bem-sucedido.
     */
    bool join(User *user);

    /**
     * @brief Remove um usuário do canal.
     * 
     * @param user_nickname apelido do usuário a ser expulso.
     * @return true caso seja bem-sucedido.
     * @return false caso o usuário especificado não exista.
     */
    bool kick(string user_nickname);

    /**
     * @brief Silencia um usuário do canal. Não é possível silenciar o administrador.
     * 
     * @param user_nickname apelido do usuário a ser silenciado.
     * @return true caso o usuário tenha sido silenciado com sucesso.
     * @return false caso o usuário especificado não tenha sido encontrado.
     */
    bool mute(string user_nickname);

    /**
     * @brief Remove o silêncio de um usuário do canal.
     * 
     * @param user_nickname apelido do usuário a ter o silêncio removido.
     * @return true caso o usuário tenha sido modificado com sucesso.
     * @return false caso o usuário especificado não tenha sido encontrado.
     */
    bool unmute(string user_nickname);
};

#endif