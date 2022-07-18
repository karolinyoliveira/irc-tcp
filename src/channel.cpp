// Dependências
#include "../lib/channel.hpp"
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


