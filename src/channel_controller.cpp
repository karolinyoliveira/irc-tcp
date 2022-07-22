// Dependências
#include "../lib/channel_controller.hpp"
#include "../lib/channel.hpp"
#include "../lib/socket.hpp"
#include "../lib/utils.hpp"
#include "../lib/readline.hpp"
#include <thread>
using namespace std;

// Lógica da thread de novas conexões
void ChannelController::connections_thread_logic() {

    // Para controle dos descritores de arquivo
    int maxFD, currFD, ready;
    const int server_file_descriptor = ChannelController::server_socket->getfileDescriptor();

    // Para navegação nas conexões
    map<int, pair<string *, User *>>::iterator connection_itr;

    // Execução enquanto não solicitar finalização
    while(ChannelController::may_exit == false) {

        // Reinicialização dos descritores de arquivo
        FD_ZERO(&(ChannelController::fdset));
        FD_SET(server_file_descriptor, &(ChannelController::fdset));
        maxFD = server_file_descriptor;

        // Verificação de todas as conexões
        for(
            connection_itr = ChannelController::connections.begin(); 
            connection_itr != ChannelController::connections.end(); 
            ++connection_itr
        ){
            // Verifica se permanece conectado
            currFD = connection_itr->first;
            if(currFD){
                FD_SET(currFD, &(ChannelController::fdset));
            }

            // Para obter o descritor de maior valor absoluto
            if(currFD > maxFD){
                maxFD = currFD;
            }
        }

        // Espera pela atividade de quaisquer sockets conectados (sem timeout)
        ready = select(maxFD + 1, &(ChannelController::fdset), NULL, NULL, NULL);
        if ((ready < 0) && (errno != EINTR)) {
            printf("Failed to retrieve the number of ready descriptors");
        }

        // Ao finalizar a espera pela preparação do socket do servidor, tenta realizar novas conexões
        if(FD_ISSET(server_file_descriptor, &(ChannelController::fdset))) {
            currFD = ChannelController::server_socket->accept();
            ChannelController::connections.insert (
                pair<int, pair<string *, User *>> (
                    currFD, 
                    make_pair<string *, User *>(NULL, NULL)
                )
            );
        }
    }
}


// Execução auxiliar para comandos administrativos
bool ChannelController::verify_credentials (
    map<int, pair<string *, User *>>::iterator connection_itr, 
    map<string, Channel *>::iterator *channel_itr, 
    bool check_admin, 
    int currFD
){

    // Dados da conexão
    string *user_channel = connection_itr->second.first;
    User *user = connection_itr->second.second;

    // Verificação de conexão
    if(user_channel != NULL && user != NULL) {

        // Busca pelo canal
        (*channel_itr) = ChannelController::channels.find(*user_channel);
        if((*channel_itr) != ChannelController::channels.end()) {

            // Verificação de administração
            if(check_admin == true) {
                if((*channel_itr)->second->is_admin(user) == true) {
                    return true;
                }

                // Falta de credenciais
                else {
                    Socket::send(currFD, "An admin role is needed in order to execute this command\n", 0);
                }
            }

            // Sem necessidade de credenciais administrativas
            else {
                return true;
            }
        }

        // Canal indisponível
        else {
            Socket::send(currFD, "Channel not found\n", 0);
        }
    }

    // Ausência de conexão estabelecida
    else {
        Socket::send(currFD, "User is not connected to a channel\n", 0);
    }

    return false;
}


// Lógica da thread de recepção de mensagens
void ChannelController::messages_thread_logic() {

    // Mensagem e apelido a serem recebidos
    string nickname, message;
    long unsigned int str_splitter;

    // Descritor de arquivo atual
    int currFD;

    // Para navegação nas conexões e nos canais
    map<int, pair<string *, User *>>::iterator connection_itr;
    map<string, Channel *>::iterator channel_itr;

    // Para controle de convites
    bool invitation = false;
    
    // Execução enquanto não solicitar finalização
    while(ChannelController::may_exit == false) {

        // Verificação de todas as conexões
        for(
            connection_itr = ChannelController::connections.begin(); 
            connection_itr != ChannelController::connections.end(); 
            ++connection_itr
        ){
            // Verifica se permanece conectado
            currFD = connection_itr->first;
            if(FD_ISSET(currFD, &fdset)) {

                // Aquisição da mensagem
                // Deve ser formatada como "apelido: mensagem"
                message = Socket::receive(currFD);
                cout << "received < " << message << endl;
                str_splitter = message.find(' ');
                nickname = message.substr(0, str_splitter - 1);
                message = message.substr(str_splitter + 1, message.length());

                // Mensagem comum
                if(message[0] != '/') {
                    
                    // Verificação de credenciais e envio da mensagem
                    if(ChannelController::verify_credentials(connection_itr, &channel_itr, false, currFD) == true) {
                        if(channel_itr->second->is_muted(nickname) == false) {
                            channel_itr->second->send_message(nickname, message);
                        }else{
                            Socket::send(currFD, "you are muted in this channel", 0);
                        }
                    }

                }

                // Execução de comandos
                else {
                    switch(message[1]) {

                        // connect
                        case 'c': {
                            break;
                        }

                        // quit
                        case 'q': {
                            Socket::send(currFD, "bye", 0);
                            close(currFD);
                            ChannelController::connections.erase(connection_itr);
                            break;
                        }

                        // ping
                        case 'p': {
                            Socket::send(currFD, "pong", 0);
                            break;
                        }
                        
                        // join
                        case 'j': {

                            // Nome do canal
                            message = message.substr(message.find(' ') + 1, message.length());

                            // Verificação de convite (+i ao final)
                            str_splitter = message.find(" +i");
                            if(str_splitter < message.length()){
                                message = message.substr(0, str_splitter);
                                invitation = true;
                            }

                            // Atualização da conexão
                            connection_itr->second.first = new string(message);
                            if(connection_itr->second.second == NULL) {
                                connection_itr->second.second = new User(nickname, currFD);
                            }

                            // Conexão ao canal
                            if( ChannelController::join_channel (
                                connection_itr->second.second, 
                                message, 
                                invitation
                            ) == true ){
                                Socket::send(currFD, "joined channel successfully", 0);
                            }else{
                                Socket::send(currFD, "failed to join channel", 0);
                            }

                            break;
                        }
                        
                        // invite
                        case 'i': {

                            // Verificação de credenciais
                            if(ChannelController::verify_credentials(connection_itr, &channel_itr, true, currFD) == true) {

                                // Apelido do convidado
                                message = message.substr(message.find(' ') + 1, message.length());

                                // Convite
                                channel_itr->second->invite(message);

                            }

                            break;
                        }
                        
                        // kick
                        case 'k': {

                            // Verificação de credenciais
                            if(ChannelController::verify_credentials(connection_itr, &channel_itr, true, currFD) == true) {

                                // Apelido do usuário a ser expulso
                                message = message.substr(message.find(' ') + 1, message.length());

                                // Expulsão
                                if(channel_itr->second->kick(message) == true){
                                    Socket::send(currFD, "kicked user successfully", 0);
                                }else{
                                    Socket::send(currFD, "failed to kick user (not found)", 0);
                                }
                                
                            }

                            break;
                        }

                        // mute
                        case 'm': {

                            // Verificação de credenciais
                            if(ChannelController::verify_credentials(connection_itr, &channel_itr, true, currFD) == true) {

                                // Apelido do usuário a ser mutado
                                message = message.substr(message.find(' ') + 1, message.length());

                                // Mute
                                if(channel_itr->second->mute(message) == true){
                                    Socket::send(currFD, "muted user successfully", 0);
                                }else{
                                    Socket::send(currFD, "failed to mute user (not found)", 0);
                                }
                                
                            }

                            break;
                        }

                        // unmute
                        case 'u': {

                            // Verificação de credenciais
                            if(ChannelController::verify_credentials(connection_itr, &channel_itr, true, currFD) == true) {

                                // Apelido do usuário a ser desmutado
                                message = message.substr(message.find(' ') + 1, message.length());

                                // Unmute
                                if(channel_itr->second->unmute(message) == true) {
                                    Socket::send(currFD, "unmuted user successfully", 0);
                                }else{
                                    Socket::send(currFD, "failed to unmute user (not found)", 0);
                                }
                                
                            }

                            break;
                        }

                        // whois
                        case 'w': {

                            // Verificação de credenciais
                            if(ChannelController::verify_credentials(connection_itr, &channel_itr, true, currFD) == true) {

                                // Apelido do usuário-alvo
                                message = message.substr(message.find(' ') + 1, message.length());

                                // Obtenção do file_descriptor
                                int whois = channel_itr->second->whois(message);
                                if(whois >= 0) {
                                    message += " is ";
                                    message += whois;
                                    message += "\n";
                                    Socket::send(currFD, message, 0);
                                }else{
                                    Socket::send(currFD, "failed to whois user (not found)", 0);
                                }
                                
                            }

                            break;
                        }
                        
                        // Nenhuma correspondência
                        default: {
                            Socket::send(currFD, "Invalid command\n", 0);
                        }
                    }
                }
            } 
        }
    }
}


// Construtor
ChannelController::ChannelController(int max_connections) {

    // Máximo de conexões
    if(max_connections <= 0){
        throw std::invalid_argument("max_connections must be greater than zero.");
    }
    ChannelController::max_connections = max_connections;

    // Socket
    ChannelController::server_socket = new Socket(PORT);
    ChannelController::server_socket->bind();
    ChannelController::server_socket->listen(max_connections);

    // Threads
    ChannelController::connections_thread = thread(&ChannelController::connections_thread_logic, this);
    ChannelController::messages_thread = thread(&ChannelController::messages_thread_logic, this);
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
        return channel_iterator->second->join(user);
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
    return true;
}


// Inicia o controlador
void ChannelController::start() {
    ChannelController::connections_thread.join();
    ChannelController::messages_thread.join();
}


// Encerra o controlador
void ChannelController::stop() {
    ChannelController::may_exit = true;
}