// Dependências
#include "../lib/channel.hpp"
#include "../lib/user.hpp"
#include <string>
#include <regex>
using namespace std;


// Construtor da classe interna
Channel::ChannelUser::ChannelUser(User *user) {
    Channel::ChannelUser::user = user;
}


// Silencia um usuário do canal
void Channel::ChannelUser::mute() {
    Channel::ChannelUser::muted = true;
}


// Remove o silêncio de um usuário do canal
void Channel::ChannelUser::unmute() {
    Channel::ChannelUser::muted = false;
}


// Construtor
Channel::Channel(string name) {

    // Validação do nome
    if(regex_match(name, regex("^[&#][^ ,]+$")) == true) {
        Channel::name = name;
    }

    // Nome inválido
    else {
        throw std::invalid_argument("Invalid name syntax for RFC-1459.");
    }
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

    // Verificação de canal vazio
    if(Channel::admin == NULL && Channel::users.size() <= 0){
        Channel::admin = user;
    }else{
        Channel::users.insert (
            pair<string, ChannelUser *> (
                user->get_nickname(), 
                new ChannelUser(user)
            )
        );
    }

    return true;
}


// Expulsa um usuário
bool Channel::kick(string user_nickname){

    // Verificação de admin
    if(Channel::admin != NULL && user_nickname == Channel::admin->get_nickname()){
        Channel::admin = NULL;
        return true;
    }

    // Verificação de usuários comuns
    map<string, ChannelUser *>::iterator user_iterator;
    user_iterator = Channel::users.find(user_nickname);
    if(user_iterator != Channel::users.end()){
        Channel::users.erase(user_iterator);
        return true;
    }

    // Usuário não encontrado
    return false;
}


// Silencia um usuário do canal
bool Channel::mute(string user_nickname) {
    map<string, ChannelUser *>::iterator user_iterator;
    user_iterator = Channel::users.find(user_nickname);
    if(user_iterator != Channel::users.end()){
        user_iterator->second->mute();
        return true;
    }
    return false;
}


// Remove o silêncio de um usuário do canal
bool Channel::unmute(string user_nickname) {
    map<string, ChannelUser *>::iterator user_iterator;
    user_iterator = Channel::users.find(user_nickname);
    if(user_iterator != Channel::users.end()){
        user_iterator->second->unmute();
        return true;
    }
    return false;
}  