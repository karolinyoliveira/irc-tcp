// Dependências
#include "../lib/user.hpp"
#include <string>
using namespace std;

// Construtor
User::User(string nickname) {
    User::nickname = nickname;
}


// Operador comparativo.
bool operator< (const User &left, const User &right){
    return left.nickname.compare(right.nickname);
}


// Retorna o apelido
string User::get_nickname() {
    return User::nickname;
}


// Define um novo apelido de usuário
void User::set_nickname(string nickname){
    User::nickname = nickname;
}