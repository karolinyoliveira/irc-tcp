// Dependências
#include "../lib/user.hpp"
#include "../lib/socket.hpp"
#include <string>
using namespace std;

// Construtor
User::User(string nickname, Socket *socket) {
    if(socket == NULL) {
        throw std::invalid_argument("socket must be specified.");
    }
    if(nickname.size() > 50) {
        throw std::invalid_argument("nickname is limited to 50 ASCII characters.");
    }
    User::nickname = nickname;
    User::socket = socket;
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
    return User::socket->getfileDescriptor();
}