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

const char* Server::WrongParameterMODE::what() const throw(){
    return("[Error] :Paramètres: #<channel> {[+|-]|o|i} [<user>]\n");
}

const char* Server::WrongParameterPRIVMSG::what() const throw(){
    return("[Error] :Paramètres: <receiver> <text to be sent>\n");
}

const char* Server::WrongParameterJOIN::what() const throw(){
    return("[Error] :Paramètres: #<channel> <password>\n");
}

const char* Server::ERR_NOTEXTTOSEND::what() const throw(){
    return("[Error] :No text to send\n");
}

const char* Server::WrongParameterKICK::what() const throw(){
    return("[Error] :Paramètres: <pseudonyme> #<canal> \n");
}

const char* Server::CommandDoesntExist::what() const throw(){
    return("[Error] :Command doesn't exist\n");
}

const char* Server::ERR_NOSUCHCHANNEL::what() const throw(){
    return("[Error] :No such channel\n");
}

const char* Server::ERR_BADCHANNELKEY::what() const throw(){
    return("[Error] :Cannot join channel wrong key\n");
}

const char* Server::ERR_USERSDONTMATCH::what() const throw(){
    return("[Error] :Client doesn't exist\n");
}

const char* Server::ERR_PASSWDMISMATCH::what() const throw(){
    return("[Error] :Password incorrect\n");
}

const char* Server::ERR_NOTONCHANNEL::what() const throw(){
    return("[Error] :You're not on that channel\n");
}

const char* Server::ERR_NICKNAMEINUSE::what() const throw(){
    return("[Error] :Nickname is already in use\n");
}

const char* Server::ERR_NONICKNAMEGIVEN::what() const throw(){
    return("[Error] :No nickname given\n");
}

std::string Server::getPassword(){
    return(password);
}

void Server::setPassword(std::string input_password){
    password = input_password;
}

void    Server::parseComma(std::string buff, std::vector<std::string> &target, size_t next_space){
    int i = 0;

    while(buff.find(',') != std::string::npos && (buff.find(',') < next_space || next_space == 0))
    {
        target.push_back(buff.substr(0,buff.find(',')));
        buff = buff.substr(buff.find(',') + 1,buff.find(' ') - buff.find(',') - 1);
        // if (find_socket(target[i]) == _server.end())
        //     throw ERR_USERSDONTMATCH();
        if(buff.find(',') == std::string::npos)
        {
            if(buff.find(' ') != std::string::npos)
                target.push_back(buff.substr(0,buff.find(' ')));
            // else
            //     throw ERR_USERSDONTMATCH();
            i++;
        }
        i++;
    }
    if(buff.find(' ') == std::string::npos)
        target.push_back(buff.substr(0,buff.size()));
    else
        target.push_back(buff.substr(0,buff.find(' ')));
}

void    Server::compareName(std::string src, int type){
    std::string target;
    std::transform(src.begin(),src.end(),src.begin(),::tolower);
    if(src.size() == 0)
        throw ERR_NONICKNAMEGIVEN();
    for(std::map<int, Client>::iterator it = _server.begin();it != _server.end();it++)
    {
        if(type == 0)
            target = it->second.getNick_name();
        else
            target = it->second.getUser_name();
        std::transform(target.begin(),target.end(),target.begin(),::tolower);
        if(src.compare(target) == 0)
            throw ERR_NICKNAMEINUSE();
    }
}


int check_mode_option(std::string option){
    if(option[0] == 'o' && option.size() == 1)
        return (0);
    if((option[0] != '-' && option[0] != '+') || (option[1] != 'i' && option[1] != 't' && option[1] != 'o'))
        return(1);
    return(0);
}

std::map<int,Client> ::iterator  Server::find_socket(std::string target){
    std::map<int,Client> ::iterator it;
    for(it = _server.begin(); it != _server.end();it++)
    {
        if (it->second.getNick_name().compare(target) == 0)
            return(it);
    }
    throw ERR_USERSDONTMATCH();
    return(it);
}


void    Server::message_receiver(std::map<int,Client>::iterator it){
    std::string buffer;
    std::string tmp;
    char buffer_tmp[2048];
    int len;

    buffer.reserve(2048);
    len = recv(it->first,buffer_tmp,sizeof(buffer_tmp),0);
    buffer.append(buffer_tmp);
    buffer.erase(len,buffer.size() - len);
    it->second.appendCommand(buffer);
    while(it->second.getCommand().find_first_of('\n') != (size_t)-1)
    {
        //do your command
        it->second.eraseBackslash_R();
        tmp = it->second.getCommand().substr(0,it->second.getCommand().find_first_of('\n'));
        tmp[tmp.size()] = 0;

        choose_cmd(tmp, it);

        it->second.eraseToBackslash_N();
        if(!it->second.getCommand().find_first_of('\n'))
            it->second.resetCommand();
    }
}

void    Server::initServer(char **av)
{
    //AF_INET = IPv4
    //SOCK_STREAM = flux de données bidirectionnel
    //0 = le protocole approprié sera choisi automatiquement en fonction du premier argument
    password.append(av[2]);
    this->password = password;
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
                message_receiver(it);
                if(_server.empty())
                    break;
            }
        }
    }
}

void Server::commandJoin(std::string buff,std::map<int,Client>::iterator client){
    int len;
    std::vector<int> channel_exist;
    std::vector<std::string> name_channel;
    std::vector<std::string> password;
    Channel instance_channel;

    channel_exist.reserve(100);
    buff = buff.substr(5,buff.size());
    len = buff.find(' ');
    if(len == -1)
        throw WrongParameterJOIN();

    parseComma(buff,name_channel,buff.find(' '));
    buff = buff.substr(buff.find(' ') + 1,buff.size());
    parseComma(buff,password,buff.find(' '));
    for(size_t i = 0; i < name_channel.size();i++)
        if(name_channel[i][0] != '#')
            throw WrongParameterJOIN();

    if(password.size() != name_channel.size())
        throw WrongParameterJOIN();
    for(size_t i = 0; i < password.size();i++)
    {
        for(std::map<std::string,Channel>::iterator it = _vchannel.begin(); it != _vchannel.end();it++)
        {
            if (name_channel[i].compare(it->first) == 0 && it->second.getInvite() == false)
            {
                if(password[i].compare(it->second.getPassword()) == 0)
                    it->second.addClient(client->first);
                else
                    throw ERR_BADCHANNELKEY();
                channel_exist[i] = 1;
                break;
            }
            channel_exist[i] = 0;
        }
    }
    for(size_t i = 0; i < password.size();i++)
    {
        if(channel_exist[i] != 1)
        {
            this->_vchannel.insert(std::pair<std::string,Channel>(name_channel[i],instance_channel));
            _vchannel[name_channel[i]].setMode();
            _vchannel[name_channel[i]].addClient(client->first);
            _vchannel[name_channel[i]].setPassword(password[i]);
        }
    }
}

void Server::commandKick(std::string buff,std::map<int,Client>::iterator client){
    std::string target;
    std::string channel;
    std::string message;
    std::map<std::string,Channel>::iterator it;

    buff = buff.substr(5,buff.size());
    int len = buff.find(' ');
    if (len == -1)
        throw WrongParameterKICK();
    
    channel = buff.substr(0,buff.find(' '));
    it = _vchannel.find(channel);
    if(it == _vchannel.end())
        throw ERR_NOSUCHCHANNEL();

    buff = buff.substr(buff.find(' ') + 1,buff.size());
    if(buff.find(' ') == std::string::npos)
        throw WrongParameterKICK();
    target = buff.substr(0,buff.find(' '));

    if (it->second.find_client(find_socket(target)->first) == 0)
        throw ERR_NOTONCHANNEL();

    buff = buff.substr(buff.find(' ') + 1,buff.size());
    message = buff.substr(0,buff.size());
    message.append("\n");
    if(it->second.isOperator(client->first) == 1)
    {
        it->second.removeClient(find_socket(target)->first);
        send(find_socket(target)->first,message.c_str(),message.size(),0);
    }
}

void Server::commandPrivMsg(std::string buff,std::map<int,Client>::iterator client){
    std::vector<std::string> target;
    std::string message;

    buff = buff.substr(8,buff.size());
    int len = buff.find(' ');
    if (len == -1)
        throw WrongParameterPRIVMSG();
    parseComma(buff,target,buff.find(' '));
    message = buff.substr(buff.find(' ') + 1, buff.size() - buff.find(' ') - 1);
    message.append("\n");
    for(int i = target.size() -1;i != -1; i--)
    {
        for(std::map<int,Client> ::iterator it = _server.begin(); it != _server.end();it++)
        {
            if (buff.find(' ') != std::string::npos && it->second.getNick_name().compare(target[i]) == 0)
            {

                send(it->first,client->second.getNick_name().c_str(),client->second.getNick_name().size(),0);
                send(it->first," :",2,0);
                send(it->first,message.c_str(),message.size(),0);
            }
        }
    }
}

void Server::executemode(std::string option, std::map<std::string,Channel>::iterator it, std::map<int,Client>::iterator client){
    if(option[0] == 'o')
        it->second.invertOperator(client->first);
    else if(option[1] == 'o' && option[0] == '+')
        it->second.addOperator(client->first);
    else if(option[1] == 'o' && option[0] == '-')
        it->second.removeOperator(client->first);
    else if(option[1] == 'i' && option[0] == '+')
        it->second.setInvite(true);
    else if(option[1] == 'i' && option[0] == '-')
        it->second.setInvite(false);
}

void Server::commandMode(std::string buff,std::map<int,Client>::iterator client){
    std::map<std::string,Channel>::iterator it;
    std::map<int,Client> ::iterator id;
    std::string option;
    std::string name_channel;
    std::string Name;
    
    (void)client;
    buff = buff.substr(5,buff.size());
    int len = buff.find(' ');
    if (len == -1)
        throw WrongParameterMODE();

    name_channel = buff.substr(0,len);
    it = _vchannel.find(name_channel);
    if(it == _vchannel.end())
        throw ERR_NOSUCHCHANNEL();

    buff = buff.substr(buff.find(' ') + 1,buff.size());
    len = buff.find(' ');
    if(len == -1)
        option = buff.substr(0,buff.size());
    else
    {
        option = buff.substr(0,len);
        buff = buff.substr(buff.find(' ') + 1,buff.size());
        Name = buff.substr(0,buff.size());
        id = find_socket(Name);
    }
    if(check_mode_option(option))
        throw WrongParameterMODE();
    executemode(option, it, id);
}

void Server::commandPart(std::string buff,std::map<int,Client>::iterator client){

    std::map<std::string,Channel>::iterator it;
    // std::string name_channel;
    std::vector<std::string> name_channel;

    buff = buff.substr(5,buff.size());
    if (buff.size() < 1)
        throw WrongParameterMODE();

    parseComma(buff,name_channel,buff.find(' '));
    for(size_t i = 0; i < name_channel.size();i++)
    {
        it = _vchannel.find(name_channel[i]);
        if(it == _vchannel.end())
            throw ERR_NOSUCHCHANNEL();
        if(!it->second.find_client(client->first))
            throw ERR_NOTONCHANNEL();
        it->second.removeClient(client->first);
        if(it->second.getNbOperator() == 0)
            _vchannel.erase(it->first);
    }
}

void Server::commandInvite(std::string buff,std::map<int,Client>::iterator client){
    std::map<std::string,Channel>::iterator it;
    std::map<int,Client> ::iterator id;
    std::string name_channel;
    std::string name_target;
    int len;
    
    buff = buff.substr(7,buff.size());
    len = buff.find(' ');
    if (len == -1)
        throw WrongParameterMODE();
    name_target = buff.substr(0,len);
    id = find_socket(name_target);
    buff = buff.substr(buff.find(' ') + 1,buff.size());
    name_channel = buff.substr(0,buff.size());

    it = _vchannel.find(name_channel);
    if(it == _vchannel.end())
        throw ERR_NOSUCHCHANNEL();

    if (client->first == it->second.getOperator())
        it->second.addClient(id->first);
}

void Server::commandNotice(std::string buff,std::map<int,Client>::iterator client){
    std::vector<std::string> target;
    std::string message;
    
    buff = buff.substr(7,buff.size());
    parseComma(buff,target,buff.find(' '));
    message = buff.substr(buff.find(' ') + 1, buff.size() - buff.find(' ') - 1);
    message.append("\n");
    for(size_t i = 0; i < target.size();i++)
    {
        for(std::map<int,Client> ::iterator it = _server.begin(); it != _server.end();it++)
        {
            if (buff.find(' ') != std::string::npos && it->second.getNick_name().compare(target[i]) == 0)
            {

                send(it->first,client->second.getNick_name().c_str(),client->second.getNick_name().size(),0);
                send(it->first," :",2,0);
                send(it->first,message.c_str(),message.size(),0);
            }
        }
    }
}

void Server::commandQuit(std::string buff,std::map<int,Client>::iterator client){
    std::map<std::string,Channel>::iterator it_c;
    std::string out;

    for (it_c = _vchannel.begin(); it_c != _vchannel.end(); it_c++)
            if (it_c->second.find_client(client->first))
                it_c->second.removeClient(client->first);
    if (buff.empty())
    {    
        out.append(client->second.getNick_name() + " quits\n");           //message par default ?
        send(client->first, out.c_str(),out.size(),0);
    }
    else
    {
        out = buff.substr(buff.find(' ') + 1, buff.size());
        out.append("\n");
        send(client->first,out.c_str(),out.size(),0);
    }
    close(client->first);
    _server.erase(client->first);
}

void Server::choose_cmd(std::string buff,std::map<int,Client>::iterator client)
{
    std::string outerror;
    try
    {
        int status = client->second.getStatus();
        if (status == 0 && buff.compare(0,5,"PASS ") == 0)
            commandPass(client , buff);
        else if (status == 1 && buff.compare(0,5,"NICK ") == 0)
            commandNick(client , buff); 
        else if (status == 2 && buff.compare(0,5,"USER ") == 0)
            commandUser(client , buff);
        else if (buff.size() > 5 && buff.substr(0,5).compare("JOIN ") == 0)
            commandJoin(buff,client);
        else if (buff.size() > 5 && buff.substr(0,5).compare("KICK ") == 0)
            commandKick(buff,client);
        else if (buff.size() > 5 && buff.substr(0,5).compare("PART ") == 0)
            commandPart(buff,client);
        else if (buff.size() > 8 && buff.substr(0,8).compare("PRIVMSG ") == 0)
            commandPrivMsg(buff,client);
        else if (buff.size() > 5 && buff.substr(0,5).compare("MODE ") == 0)
            commandMode(buff,client);
        else if (buff.size() > 7 && buff.substr(0,7).compare("INVITE ") == 0)
            commandInvite(buff,client);
        else if (buff.size() > 7 && buff.substr(0,7).compare("NOTICE ") == 0)
            commandNotice(buff,client);
        else if (buff.size() > 5 && buff.substr(0,5).compare("QUIT ") == 0)
            commandQuit(buff,client);
        else
            throw CommandDoesntExist();
    }
    catch(const std::exception& e)
    {
        outerror.append(e.what());
        send(client->first,outerror.c_str(),outerror.size(),0);
    }
    
}

void Server::commandPass(std::map<int,Client>::iterator& it, std::string buffer){
    std::string outputstr("[1] [PASS CORRECT]\n");

    buffer = buffer.substr(5,buffer.size() - 5);
    if (this->password.compare(buffer) != 0)
        throw ERR_PASSWDMISMATCH();
    else
    {
        it->second.setStatus(1);
        send(it->first,outputstr.c_str(),outputstr.size(),0);
    }
}

void Server::commandNick(std::map<int,Client>::iterator& it, std::string buffer){
    std::string outputstr("[2] [NICK NAME SET] : ");

    buffer = buffer.substr(5,buffer.size() - 5);
    compareName(buffer, 0);
    it->second.setNickName(buffer);
    it->second.setStatus(2);
    outputstr.append(it->second.getNick_name());
    outputstr.append("\n");
    send(it->first, outputstr.c_str() ,outputstr.size(),0);
}

void Server::commandUser(std::map<int,Client>::iterator& it, std::string buffer){
    std::string outputstr("[3] [USER NAME SET] : ");
    
    buffer = buffer.substr(5,buffer.size() - 5);
    compareName(buffer, 1);
    it->second.setUserName(buffer);
    it->second.setStatus(3);
    outputstr.append(it->second.getUser_name());
    outputstr.append("\n");
    send(it->first,outputstr.c_str(),outputstr.size(),0);
}
