// Dependências
#include "../lib/channel.hpp"
#include "../lib/user.hpp"
#include <string>
#include <regex>
using namespace std;

// Construtor
Channel::Channel(string name, bool invited_only) {

    // Validação do nome
    /*
    if(regex_match(name, regex("^[&#][^ ,]+$")) == true) {
        Channel::name = name;
        Channel::invited_only = invited_only;
    }
    // Nome inválido
    else {
        throw std::invalid_argument("Invalid name syntax for RFC-1459.");
    }
    */
    Channel::name = name;
    Channel::invited_only = invited_only;
}


// Operador comparativo.
bool operator< (const Channel &left, const Channel &right){
    return left.name.compare(right.name);
}


// Getter do nome
string Channel::get_name() {
    return Channel::name;
}


// Junção ao canal.
bool Channel::join(User *user) {

    // Verificação de parâmetro
    if(user == NULL){
        throw std::invalid_argument("user must be specified.");
    }

    // Verificação de canal vazio: atribuição administrativa
    if(Channel::admin == NULL && Channel::users.size() <= 0){
        Channel::admin = user;
    }
    
    // Inserção de usuário comum
    else {

        // Apelido do usuário
        string nickname = user->get_nickname();

        // Verifica se o usuário já existe previamente
        map<string, ChannelUser *>::iterator user_iterator = Channel::users.find(nickname);
        if(user_iterator != Channel::users.end()){

            // Caso o servidor não se restrinja somente a convidados
            if(Channel::invited_only == false){
                Channel::users.insert (
                    pair<string, ChannelUser *> (
                        nickname, 
                        new ChannelUser(user)
                    )
                );
            }

            // Servidor somente para convidados
            else {

                // Verifica se o usuário foi convidado
                set<string>::iterator invitation_iterator = Channel::invitations.find(nickname);
                if(invitation_iterator != Channel::invitations.end()) {
                    Channel::users.insert (
                        pair<string, ChannelUser *> (
                            nickname, 
                            new ChannelUser(user)
                        )
                    );
                }

                // Usuário não convidado
                else {
                    return false;
                }
            }
        }
    }

    // Conexão bem-sucedida
    return true;
}


// Verifa se o usuário é administrador
bool Channel::is_admin(User *user) {
    return user->get_file_descriptor() == Channel::admin->get_file_descriptor();
}


// Verifa se o usuário está mutado
bool Channel::is_muted(string user_nickname) {
    map<string, ChannelUser *>::iterator user_iterator = Channel::users.find(user_nickname);
    if(user_iterator != Channel::users.end()){
        return user_iterator->second->is_muted();
    }
    return false;
}


// Expulsa um usuário
bool Channel::kick(string user_nickname){

    // Verificação de admin
    if(Channel::admin != NULL && user_nickname == Channel::admin->get_nickname()){
        Channel::admin = NULL;
        return true;
    }

    // Verificação de usuários comuns
    map<string, ChannelUser *>::iterator user_iterator = Channel::users.find(user_nickname);
    if(user_iterator != Channel::users.end()){
        Channel::users.erase(user_iterator);
        return true;
    }

    // Usuário não encontrado
    return false;
}


// Silencia um usuário do canal
bool Channel::mute(string user_nickname) {
    map<string, ChannelUser *>::iterator user_iterator = Channel::users.find(user_nickname);
    if(user_iterator != Channel::users.end()){
        user_iterator->second->mute();
        return true;
    }
    return false;
}


// Remove o silêncio de um usuário do canal
bool Channel::unmute(string user_nickname) {
    map<string, ChannelUser *>::iterator user_iterator = Channel::users.find(user_nickname);
    if(user_iterator != Channel::users.end()){
        user_iterator->second->unmute();
        return true;
    }
    return false;
}  


// Convida um usuário para o canal
bool Channel::invite(string user_nickname) {

    // Caso o modo +i esteja inativo
    if(Channel::invited_only == false) {
        return false;
    }
    
    // Adição do convite
    Channel::invitations.insert(user_nickname);
    return true;
}


// Retorna o file_descriptor do usuário
int Channel::whois(string user_nickname) {
    map<string, ChannelUser *>::iterator user_iterator = Channel::users.find(user_nickname);
    if(user_iterator != Channel::users.end()){
        return user_iterator->second->get_file_descriptor();
    }
    return -1;
}


// Envia uma mensagem a todos os usuários
void Channel::send_message(string user_nickname, string message) {

    // Mensagem final
    string full_message = user_nickname + ": " + message;

    // Envia ao admin
    if(user_nickname != Channel::admin->get_nickname()) {
        Socket::send(Channel::admin->get_file_descriptor(), full_message, 0);
    }

    // Envia aos demais usuários
    map<string, ChannelUser *>::iterator user_iterator;
    for(
        user_iterator = Channel::users.begin(); 
        user_iterator != Channel::users.end(); 
        ++user_iterator
    ){
        Socket::send(user_iterator->second->get_file_descriptor(), full_message, 0);
    }
}