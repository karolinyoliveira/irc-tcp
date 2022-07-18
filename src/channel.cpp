// Dependências
#include "../lib/channel.hpp"
#include "../lib/user.hpp"
#include <string>
#include <regex>
using namespace std;

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


// Junção ao canal.
void Channel::join(User *user) {

    // Verificação de parâmetro
    if(user == NULL){
        throw std::invalid_argument("user must be specified.");
    }

    // Verificação de canal vazio
    if(Channel::admin == NULL && Channel::users.size() <= 0){
        Channel::admin = user;
    }else{
        Channel::users.insert(user);
    }
}