// Dependências
#include "../lib/channel_controller.hpp"
#include "../lib/channel.hpp"
using namespace std;

// Junção ao canal
bool ChannelController::join_channel(User *user, string channel_name, bool need_invitation) {

    // Verificação paramétrica
    if(user == NULL) {
        throw std::invalid_argument("user must be specified.");
    }

    // Busca pelo canal
    map<string, Channel *>::iterator channel_iterator;
    channel_iterator = ChannelController::channels.find(channel_name);
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