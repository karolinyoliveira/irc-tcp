// Dependências
#include "../lib/user.hpp"
#include "../lib/socket.hpp"
#include <string>
#include <regex>
using namespace std;

// Construtor
User::User(string nickname, int file_descriptor) {
    if(file_descriptor <= 0) {
        throw std::invalid_argument("file_descriptor must be greater than zero.");
    }
    if(nickname.size() > 50) {
        throw std::invalid_argument("nickname is limited to 50 ASCII characters.");
    }
    /*
    if(regex_match(nickname, regex("^[A-Za-z0-9_]+$")) == true){
        throw std::invalid_argument("nickname is badly formatted");
    }
    */
    User::nickname = nickname;
    User::file_descriptor = file_descriptor;
}


// Operador comparativo.
bool operator< (const User &left, const User &right){
    return left.nickname.compare(right.nickname);
}


// Retorna o apelido
string User::get_nickname() {
    return User::nickname;
}


// Retorna o fileDescriptor
int User::get_file_descriptor() {
    return User::file_descriptor;
}