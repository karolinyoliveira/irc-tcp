// Para controle de pré-compilação
#ifndef CHANNEL_CONTROLLER_HPP
#define CHANNEL_CONTROLLER_HPP

// Dependências
#include "./channel.hpp"
#include <set>
using namespace std;

/**
 * @brief Controlador de canais de comunicação RFC-1459.
 */
class ChannelController {
private:
    
    // Canais
    set<Channel *> channels;

public:
    
    /**
     * @brief Construtor para um novo objeto controlador.
     */
    ChannelController();

};

#endif