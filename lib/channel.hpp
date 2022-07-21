// Para controle de pré-compilação
#ifndef CHANNEL_HPP
#define CHANNEL_HPP

// Dependências
#include "./user.hpp"
#include <string>
#include <map>
#include <set>
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
        ChannelUser(User *user) {
            this->user = user;
        }

        // Retorna se está ou não mutado
        bool is_muted() {
            return this->muted;
        }

        // Retorna o file_descriptor
        int get_file_descriptor() {
            return this->user->get_file_descriptor();
        }

        // Muta o usuário
        void mute() {
            this->muted = true;
        }

        // Desmuta o usuário
        void unmute() {
            this->muted = false;
        }

    };
    
    // Nome do canal
    string name;

    // Usuário administrador
    User *admin = NULL;

    // Demais usuários
    map<string, ChannelUser *> users;

    // Controle de convites
    bool invited_only;
    set<string> invitations;

public:
    
    /**
     * @brief Construtor para um novo objeto de canal.
     * @param name nome do canal segundo as restrições RFC-1459.
     * @param invited_only indica se o canal é somente para convidados.
     * @throws std::invalid_argument caso o nome não siga a restrições RFC-1459.
     */
    Channel(string name, bool invited_only);

    /// Operador de comparação.
    friend bool operator< (const Channel &left, const Channel &right);

    /// Getter do nome do canal.
    string get_name();

    /** 
     * Pedido de junção ao canal.
     * @throws std::invalid_argument caso seja informado um ponteiro nulo.
     * @returns verdadeiro caso seja bem-sucedido.
     */
    bool join(User *user);

    /**
     * @brief Indica se o usuário especificado é administrador do canal.
     * 
     * @param user usuário a ser verificado.
     */
    bool is_admin(User *user);

    /**
     * @brief Indica se o usuário especificado está mutado.
     * 
     * @param user_nickname apelido do usuário a ser verificado.
     */
    bool is_muted(string user_nickname);

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

    /**
     * @brief Convida um usuário para o canal caso o modo +i esteja ativo.
     * 
     * @param user_nickname apelido do usuário a ser convidado.
     * @return true caso o usuário tenha sido convidado com sucesso.
     * @return false caso o modo +i esteja inativo para o canal.
     */
    bool invite(string user_nickname);

    /**
     * @brief Retorna o file_descriptor do usuário especificado caso encontrado; 
     * em situação contrária, retorna -1.
     * 
     * @param user_nickname apelido do usuário a ser verificado.
     */
    int whois(string user_nickname);

    /**
     * @brief Envia uma mensagem a todos os usuários do canal.
     * 
     * @param user_nickname apelido do usuário que está a enviar a mensagem.
     * @param message corpo da mensagem.
     */
    void send_message(string user_nickname, string message);
};

#endif