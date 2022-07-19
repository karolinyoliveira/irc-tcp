// Para controle de pré-compilação
#ifndef CHANNEL_CONTROLLER_HPP
#define CHANNEL_CONTROLLER_HPP

// Dependências
#include "./channel.hpp"
#include <map>
using namespace std;

/**
 * @brief Controlador de canais de comunicação RFC-1459.
 */
class ChannelController {
private:
    
    // Canais
    map<string, Channel *> channels;

public:
    
    /**
     * @brief Construtor para um novo objeto controlador.
     */
    ChannelController();

    /**
     * @brief Conecta um usuário ao canal especificado. Caso não exista tal 
     * canal previamente, um novo é criado com base no nome informado.
     * 
     * @param channel_name nome do canal ao qual conectar.
     * @return true caso a conexão seja bem-sucedida.
     * @return false caso o canal solicitado seja inexistente.
     * @throws std::invalid_argument caso seja informado um ponteiro nulo.
     */
    bool join_channel(User *user, string channel_name);
};

#endif