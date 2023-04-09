#include "Server.hpp"

Server::Server()
{

}

Server::Server(Server const & src)
{
    *this = src;
}

Server &Server::operator=(Server const & rhs)
{
    (void)rhs;
    return (*this);
}

Server::~Server()
{

}


void    Server::initServer(char **av)
{
    //AF_INET = IPv4
    //SOCK_STREAM = flux de données bidirectionnel
    //0 = le protocole approprié sera choisi automatiquement en fonction du premier argument
    std::string password("PASS ");
    password.append(av[2]);
    std::map<int,Client>::iterator it;
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in  server_addr;
    struct sockaddr_in  tmp_client_addr;
    socklen_t           tmp_client_len;
    //Table 2-1 Protocol Family
    server_addr.sin_family = AF_INET;   /*Internet domain */

    server_addr.sin_port = htons(atoi(av[1])); /*port du server*/
    
    //Stocke une adresse IP
    server_addr.sin_addr.s_addr = INADDR_ANY; /*accepte n'importe quelle adresse */

    bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_sock, 10);

    //fd pour select()
    fd_set read_fds;
    
    //slot de clients
    // int client_socks[MAX_CLIENTS] = {0};

    //number de fd total a check pour select()
    int maxfd = server_sock;

    while (1) {

        //setter server_socket, client_socket
        FD_ZERO(&read_fds);
        FD_SET(server_sock, &read_fds);

        for (it = _server.begin(); it != _server.end(); it++) {
            if (it->first > 0){
                FD_SET(it->first, &read_fds);
                maxfd = (it->first > maxfd) ? it->first : maxfd;
            }
        }

        /*select() fait la difference entre une entre client ou _server */
        select(maxfd + 1, &read_fds, NULL, NULL, NULL);

        //checker de changement au nv du socket_server
        if (FD_ISSET(server_sock, &read_fds))
        {
            int tmp_client_socket;
            tmp_client_len = sizeof(tmp_client_addr);
            tmp_client_socket = accept(server_sock,(struct sockaddr*)&tmp_client_addr, &tmp_client_len);
            Client test;
            _server.insert(std::pair<int,Client>(tmp_client_socket,test));
        }

        //checker de changement au nv du socket_client
        for (it = _server.begin();it != _server.end(); it++) {
            if (FD_ISSET(it->first, &read_fds)) {
                message_receiver(it,password);
                if(_server.empty())
                    break;
            }
        }
    }
}

void Server::commandSend(std::string buff,std::map<int,Client>::iterator client){
    buff = buff.substr(5,buff.size());
    std::string channel = buff.substr(0,buff.find(' '));
    for(std::map<std::string,Channel>::iterator it = _vchannel.begin(); it != _vchannel.end();it++)
    {
        if (buff.find(' ') != std::string::npos && channel.compare(it->first) == 0)
            it->second.sendMessage(client->second.getNick_name(),client->first,buff);
    }
}


void Server::commandJoin(std::string buff,std::map<int,Client>::iterator client){
    int len;
    buff = buff.substr(5,buff.size());
    len = buff.find(' ');
    if (len == -1){
        send(client->first,"[Error]JOIN <channel> <password> ",33,0);
        return;
    }
    for(std::map<std::string,Channel>::iterator it = _vchannel.begin(); it != _vchannel.end();it++)
    {
        if (buff.substr(0,len).compare(it->first) == 0 && buff.substr(len + 1,buff.size() - len - 1).compare(it->second.getPassword()) == 0)
        {
            it->second.addClient(client->first);
            return;
        }
    }
    Channel instance_channel;
    std::string name_channel = buff.substr(0,len);
    std::string password = buff.substr(len + 1,buff.size() - len - 1);

    this->_vchannel.insert(std::pair<std::string,Channel>(name_channel,instance_channel));//push_back(Channel(it->first,buff.substr(0,buff.find(' '))));
    _vchannel[name_channel].addClient(client->first);
    _vchannel[name_channel].setPassword(password);
}

std::map<int,Client> ::iterator  Server::find_socket_by_nick_name(std::string target){
    std::map<int,Client> ::iterator it;
    for(it = _server.begin(); it != _server.end();it++)
    {
        if (it->second.getNick_name().compare(target) == 0)
            return(it);
    }
    return(it);
}

void Server::commandKick(std::string buff,std::map<int,Client>::iterator client){
    buff = buff.substr(5,buff.size());
    int len = buff.find(' ');
    if (len == -1){
        send(client->first,"[Error]KICK <client> <channel> <message> ",41,0);
        return;
    }
    std::string target = buff.substr(0,len);
    buff = buff.substr(buff.find(' ') + 1,buff.size());
    len = buff.find(' ');
    if (len == -1){
        send(client->first,"[Error]KICK <client> <channel> <message> ",41,0);
        return;
    }
    std::string channel = buff.substr(0,buff.find(' '));
    buff = buff.substr(buff.find(' ') + 1,buff.size());
    std::string message = buff.substr(0,buff.size());

    std::map<std::string,Channel>::iterator it = _vchannel.find(channel);
    if(it == _vchannel.end())
    {
        send(client->first,"[Error] Cannot found channel",28,0);
        return ;
    }
    it->second.removeClient(find_socket_by_nick_name(target)->first, client->first);
}

void Server::commandPrivMsg(std::string buff,std::map<int,Client>::iterator client){
    buff = buff.substr(8,buff.size());
    std::string target = buff.substr(0,buff.find(' '));
    std::string message = buff.substr(buff.find(' ') + 1, buff.size() - buff.find(' ') - 1);
    for(std::map<int,Client> ::iterator it = _server.begin(); it != _server.end();it++)
    {
        if (buff.find(' ') != std::string::npos && it->second.getNick_name().compare(target) == 0)
        {

            send(it->first,client->second.getNick_name().c_str(),client->second.getNick_name().size(),0);
            send(it->first," :",2,0);
            send(it->first,message.c_str(),message.size(),0);
        }
    }
}

void Server::choose_cmd(std::string buff,std::map<int,Client>::iterator it)
{
    if (buff.size() > 5 && buff.substr(0,5).compare("SEND ") == 0)
        commandSend(buff,it);
    else if (buff.size() > 5 && (buff.substr(0,5).compare("JOIN ") && buff.substr(0,6).compare(":JOIN ")) == 0)
        commandJoin(buff,it);
    else if (buff.size() > 5 && buff.substr(0,5).compare("KICK ") == 0)
        commandKick(buff,it);
    else if (buff.size() > 8 && buff.substr(0,8).compare("PRIVMSG ") == 0)
        commandPrivMsg(buff,it);
}

void    Server::message_receiver(std::map<int,Client>::iterator it, std::string password){

    //buffer de copy
    std::string buffer;
    buffer.reserve(2048);
    char buffer_tmp[2048];
    int len;
    
    (void)password;
    len = recv(it->first,buffer_tmp,sizeof(buffer_tmp),0);
    buffer.append(buffer_tmp);
    buffer.erase(len,buffer.size() - len);
    it->second.appendCommand(buffer);
    while(it->second.getCommand().find_first_of('\n') != (size_t)-1)
    {
        //do your command
        int status = it->second.getStatus();
        it->second.eraseBackslash_R();
        std::string tmp = it->second.getCommand().substr(0,it->second.getCommand().find_first_of('\n'));
        if(status == 0)
            check_password(it ,password, tmp);
        else if(status == 1)
            check_nick_name(it , tmp); 
        else if(status == 2)
            check_user_name(it , tmp);
        else
            choose_cmd(tmp, it);
        it->second.eraseToBackslash_N();
        if(!it->second.getCommand().find_first_of('\n'))
            it->second.resetCommand();
    }
}

void Server::erase_map_element(std::map<int,Client>::iterator& it){
    //std::cout << "["<< RED << "WRONG INPUT" << RESET << "]" << std::endl;
    send(it->first,"[WRONG INPUT]\n",14,0);
}

void Server::check_password(std::map<int,Client>::iterator& it, std::string password, std::string buffer){
    if(password.compare(buffer) != 0)
        erase_map_element(it);
    else
    {
        std::string outputstr("[1] [PASS CORRECT]\n");
        it->second.setStatus(1);
        send(it->first,outputstr.c_str(),outputstr.size(),0);
    }
}

void Server::check_nick_name(std::map<int,Client>::iterator& it, std::string buffer){
    if(buffer.size() < 5 || buffer.compare(0,5,"NICK "))
        erase_map_element(it);
    else
    {
        it->second.setNickName(buffer.substr(5,buffer.size() - 5));
        it->second.setStatus(2);
        std::string outputstr("[2] [NICK NAME SET]");
        outputstr.append(it->second.getNick_name());
        outputstr.append("\n");
        send(it->first, outputstr.c_str() ,outputstr.size(),0);
    }
}

void Server::check_user_name(std::map<int,Client>::iterator& it, std::string buffer){
    if(buffer.size() < 5 || buffer.compare(0,5,"USER "))
        erase_map_element(it);
    else
    {
        std::string outputstr("[3] [USER NAME SET]");
        it->second.setUserName(buffer.substr(5,buffer.size() - 5));
        it->second.setStatus(3);
        outputstr.append(it->second.getUser_name());
        outputstr.append("\n");
        send(it->first,outputstr.c_str(),outputstr.size(),0);
    }
}
