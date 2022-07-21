// Dependências
#include "../lib/channel_controller.hpp"
#include "../lib/channel.hpp"
#include "../lib/utils.hpp"
#include "../lib/readline.hpp"
using namespace std;

// Lógica da thread de novas conexões
void ChannelController::connections_thread_logic() {

    // Para controle de conexões
    fd_set fdset;
    int maxFD, currFD, ready;

    // Execução enquanto não solicitar finalização
    while(ChannelController::may_exit == false) {

    }
}


// Lógica da thread de recepção de mensagens
void ChannelController::connections_thread_logic() {

    // Mensagem a ser recebida
    string message;
    
    // Execução enquanto não solicitar finalização
    while(ChannelController::may_exit == false) {

    }
}


// Construtor
ChannelController::ChannelController(int max_connections) {
    if(max_connections <= 0){
        throw std::invalid_argument("max_connections must be equal to or greater than zero.")
    }
    ChannelController::max_connections = max_connections;
}


// Junção ao canal
bool ChannelController::join_channel(User *user, string channel_name, bool need_invitation) {

    // Verificação paramétrica
    if(user == NULL) {
        throw std::invalid_argument("user must be specified.");
    }

    // Busca pelo canal
    map<string, Channel *>::iterator channel_iterator = ChannelController::channels.find(channel_name);
    if(channel_iterator != ChannelController::channels.end()){
        channel_iterator->second->join(user);
        return true;
    }

    // Cria-se um novo canal
    Channel *channel = new Channel(channel_name, need_invitation);
    channel->join(user);
    ChannelController::channels.insert (
        pair<string, Channel *> (
            channel->get_name(), 
            channel
        )
    );
    return false;
}


// Encerra o controlador
void ChannelController::stop() {
    ChannelController::may_exit = true;
}