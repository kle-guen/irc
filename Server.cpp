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

const char* Server::ERR_NEEDMOREPARAMS::what() const throw(){
    if(error_code == PASS)
        return("[Error] :PASS <password>\n");
    else if(error_code == NICK)
        return("[Error] :NICK <password>\n");
    else if(error_code == USER)
        return("[Error] :USER <password>\n");
    else if(error_code == PRIVMSG)
        return("[Error] :PRIVMSG <receiver> <text to be sent>\n");
    else if(error_code == JOIN)
        return("[Error] :JOIN <channel> <password>\n");
    else if(error_code == MODE)
        return("[Error] :MODE <channel> {[+|-]|o|i} [<user>]\n");
    else if(error_code == KICK)
        return("[Error] :KICK: <pseudonyme> <canal> \n");
    else if(error_code == PART)
        return("[Error] :PART: <channel> \n");
    else if(error_code == INVITE)
        return("[Error] :INVITE: <nickname> <channel>\n");
    else if(error_code == TOPIC)
        return("[Error] :INVITE: <channel> [<topic>]\n");
    else if(error_code == NOTICE)
        return("[Error] :NOTICE: <nickname> <channel>\n");
    else if(error_code == QUIT)
        return("[Error] :QUIT: [<Quit message>]\n");
    return("[Error] :default");
}

const char* Server::ERR_NOTEXTTOSEND::what() const throw(){
    return("[Error] :No text to send\n");
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

void    Server::parseComma(std::string buff, std::vector<std::string> &target){
    int i = 0;

    while(buff.find(',') != std::string::npos)
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

int    Server::compareName(std::string src, int type){
    std::string target;
    std::transform(src.begin(),src.end(),src.begin(),::tolower);
    for(std::map<int, Client>::iterator it = _server.begin();it != _server.end();it++)
    {
        if(type == 0)
            target = it->second.getNick_name();
        else
            target = it->second.getUser_name();
        std::transform(target.begin(),target.end(),target.begin(),::tolower);
        if(src.compare(target) == 0)
            return(1);
    }
    return(0);
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
            send(tmp_client_socket,"PASS <password>\n",16,0);
            _server[tmp_client_socket].setStatus(0);
        }

        //checker de changement au nv du socket_client
        for (it = _server.begin();it != _server.end(); it++) {
            size_t tmp_size = _server.size();
            if (FD_ISSET(it->first, &read_fds)) {
                message_receiver(it);
                if(_server.empty())
                    break;
                if(tmp_size != _server.size())
                    it = _server.begin();
            }
        }
    }
}

void Server::commandJoin(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::vector<int> channel_exist;
    std::vector<std::string> name_channel;
    std::vector<std::string> password;
    Channel instance_channel;
    std::string message_add("Client Added\n");
    std::string message_creat("Channel Created\n");

    channel_exist.reserve(100);
    if(cmd.size() != 3)
        throw ERR_NEEDMOREPARAMS(JOIN);

    parseComma(cmd[1],name_channel);
    parseComma(cmd[2],password);

    for(size_t i = 0; i < name_channel.size();i++)
        if(name_channel[i][0] != '#')
            throw ERR_NEEDMOREPARAMS(JOIN);

    if(password.size() != name_channel.size())
        throw ERR_NEEDMOREPARAMS(JOIN);
    for(size_t i = 0; i < password.size();i++)
    {
        for(std::map<std::string,Channel>::iterator it = _vchannel.begin(); it != _vchannel.end();it++)
        {
            if (name_channel[i].compare(it->first) == 0 && it->second.getInvite() == false)
            {
                if(password[i].compare(it->second.getPassword()) == 0)
                {
                    send(client->first,message_add.c_str(),message_add.size(),0);
                    it->second.addClient(client->first);
                    if(it->second.getTopic() == 1)
                        send(client->first,it->second.getTopicString().c_str(),it->second.getTopicString().size(),0);
                }
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
            send(client->first,message_creat.c_str(),message_creat.size(),0);
            this->_vchannel.insert(std::pair<std::string,Channel>(name_channel[i],instance_channel));
            _vchannel[name_channel[i]].setMode();
            _vchannel[name_channel[i]].addClient(client->first);
            _vchannel[name_channel[i]].setPassword(password[i]);
        }
    }
}

void Server::commandTopic( std::vector<std::string> cmd,std::map<int,Client>::iterator& client){
    std::map<std::string,Channel>::iterator it;
    std::string message_to_client("Command TOPIC executed\n");
    if(cmd.size() != 3 && cmd.size() != 2)
        throw ERR_NEEDMOREPARAMS(TOPIC);
    
    it = _vchannel.find(cmd[1]);
    if(it == _vchannel.end())
        throw ERR_NOSUCHCHANNEL();
    if(it->second.isOperator(client->first) && cmd.size() == 3 && it->second.getTopic() == 1)
    {
        cmd[2].append("\n");
        it->second.setTopicMessage(cmd[2]);
        send(client->first,message_to_client.c_str(),message_to_client.size(),0);
    }
    else if(cmd.size() == 2)
        send(client->first,it->second.getTopicString().c_str(),it->second.getTopicString().size(),0);
}

void Server::commandKick(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::string message_to_client("Command KICK executed\n");
    std::map<std::string,Channel>::iterator it;

    if(cmd.size() != 4)
    throw ERR_NEEDMOREPARAMS(KICK);
    
    it = _vchannel.find(cmd[1]);
    if(it == _vchannel.end())
        throw ERR_NOSUCHCHANNEL();

    if (it->second.find_client(find_socket(cmd[2])->first) == 0)
        throw ERR_NOTONCHANNEL();

    cmd[3].append("\n");
    if(it->second.isOperator(client->first) == 1)
    {
        send(client->first,message_to_client.c_str(),message_to_client.size(),0);
        it->second.removeClient(find_socket(cmd[2])->first);
        send(find_socket(cmd[2])->first,cmd[3].c_str(),cmd[3].size(),0);
    }
}

void Server::commandPrivMsg(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::string message_to_client("Command PRIVMSG executed\n");
    std::vector<std::string> target;
    std::string message;

    if(cmd.size() != 3)
        throw ERR_NEEDMOREPARAMS(PRIVMSG);

    parseComma(cmd[1],target);

    cmd[2].append("\n");
    for(int i = target.size() -1;i != -1; i--)
    {
        for(std::map<int,Client> ::iterator it = _server.begin(); it != _server.end();it++)
        {
            if (it->second.getNick_name().compare(target[i]) == 0)
            {
                send(client->first,message_to_client.c_str(),message_to_client.size(),0);
                send(it->first,client->second.getNick_name().c_str(),client->second.getNick_name().size(),0);
                send(it->first," :",2,0);
                send(it->first,message.c_str(),message.size(),0);
            }
        }
    }
}

void Server::executemode(std::string option, std::map<std::string,Channel>::iterator it, std::map<int,Client>::iterator client){
    if(option[0] == 'o' && option.size() == 1)
        it->second.invertOperator(client->first);
    else if(option[1] == 'o' && option[0] == '+')
        it->second.addOperator(client->first);
    else if(option[1] == 'o' && option[0] == '-')
        it->second.removeOperator(client->first);
    if(option[0] == 'i' && option.size() == 1)
    {
        if(it->second.getInvite() == 1)
            it->second.setInvite(0);
        else
            it->second.setInvite(1);
    }
    else if(option[1] == 'i' && option[0] == '+')
        it->second.setInvite(true);
    else if(option[1] == 'i' && option[0] == '-')
        it->second.setInvite(false);

    if(option[0] == 't' && option.size() == 1)
    {
        if(it->second.getTopic() == 1)
            it->second.setTopic(0);
        else
            it->second.setTopic(1);
    }
    else if(option[1] == 't' && option[0] == '-')
        it->second.setTopic(false);
    else if(option[1] == 't' && option[0] == '+')
        it->second.setTopic(true);
    else
        return ;
}

void Server::commandMode(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::string message_to_client("Command MODE executed\n");
    std::map<std::string,Channel>::iterator it;
    std::map<int,Client> ::iterator id;
    
    (void)client;
    if(cmd.size() != 4 && cmd.size() != 3)
    {
        _type_error = 1;
        throw ERR_NEEDMOREPARAMS(MODE);
    }
    it = _vchannel.find(cmd[1]);
    if(it == _vchannel.end())
        throw ERR_NOSUCHCHANNEL();

    if(cmd.size() == 4)
        id = find_socket(cmd[3]);
    if(check_mode_option(cmd[2]))
        throw ERR_NEEDMOREPARAMS(MODE);
    executemode(cmd[2], it, id);
    
    send(client->first,message_to_client.c_str(),message_to_client.size(),0);
}

void Server::commandPart(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::string message_to_client("Command PART executed\n");

    std::map<std::string,Channel>::iterator it;
    // std::string name_channel;
    std::vector<std::string> name_channel;

    if(cmd.size() != 3)
        throw ERR_NEEDMOREPARAMS(PART);

    parseComma(cmd[1],name_channel);
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
        send(client->first,message_to_client.c_str(),message_to_client.size(),0);
    }
}

void Server::commandInvite(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::string message_to_client("Command INVITE executed\n");
    std::map<std::string,Channel>::iterator it;
    std::map<int,Client> ::iterator id;
    std::string name_channel;
    std::string name_target;

    if(cmd.size() != 3)
        throw ERR_NEEDMOREPARAMS(INVITE);

    id = find_socket(cmd[1]);

    it = _vchannel.find(cmd[2]);
    if(it == _vchannel.end())
        throw ERR_NOSUCHCHANNEL();

    if (client->first == it->second.getOperator())
    {
        it->second.addClient(id->first);
        send(client->first,message_to_client.c_str(),message_to_client.size(),0);
    }
}

void Server::commandNotice(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::string message_to_client("Command NOTICE executed\n");
    std::vector<std::string> target;
    std::string message;
    
    if(cmd.size() != 2)
        throw ERR_NOSUCHCHANNEL();
    parseComma(cmd[1],target);
    cmd[2].append("\n");
    for(size_t i = 0; i < target.size();i++)
    {
        for(std::map<int,Client> ::iterator it = _server.begin(); it != _server.end();it++)
        {
            if (it->second.getNick_name().compare(target[i]) == 0)
            {
                send(client->first,message_to_client.c_str(),message_to_client.size(),0);
                send(it->first,client->second.getNick_name().c_str(),client->second.getNick_name().size(),0);
                send(it->first," :",2,0);
                send(it->first,message.c_str(),message.size(),0);
            }
        }
    }
}

void Server::commandQuit(std::string buff,std::map<int,Client>::iterator client){
    std::string message_to_client("Command QUIT executed\n");
    std::map<std::string,Channel>::iterator it_c;
    std::string out;

    for (it_c = _vchannel.begin(); it_c != _vchannel.end();)
    {
        if (it_c->second.find_client(client->first))
        {
            it_c->second.removeClient(client->first);
            if (it_c->second.getNbOperator() == 0)
            {
                _vchannel.erase(it_c->first);
                if (_vchannel.empty())
                    break ;
            }
            else
                ++it_c;
        }
    }
    if (buff.size() == 4)
    {
        out.append(client->second.getNick_name() + " left\n");
        sendFromClient(client, out);
    }
    else
    {
        out = buff.substr(buff.find(' ') + 1, buff.size());
        out.append("\n");
        sendFromClient(client, out);
    }
    send(client->first,message_to_client.c_str(),message_to_client.size(),0);
    close(client->first);
    _server.erase(client->first);
}

std::vector<std::string> split(std::string str, char delimiter) {
  std::vector<std::string> tokens;
  size_t start = 0, end = 0;

  while ((end = str.find(delimiter, start)) != std::string::npos) {
    tokens.push_back(str.substr(start, end - start));
    start = end + 1;
  }

  tokens.push_back(str.substr(start));
  return tokens;
}

void Server::sendFromClient(std::map<int,Client>::iterator client, std::string message)
{
    for(std::map<int,Client> ::iterator it = _server.begin(); it != _server.end();it++)
    {
        if (it != client)
            send(it->first, message.c_str(), message.size(),0);
    }
    std::cout << message;
}

void Server::choose_cmd(std::string buff,std::map<int,Client>::iterator client)
{
    std::string outerror;
    std::vector<std::string> cmd;
    cmd = split(buff,' ');
    try
    {
        int status = client->second.getStatus();
        if (status == 0 && buff.compare(0,5,"PASS ") == 0)
            commandPass(client , cmd);
        else if (status == 1 && buff.compare(0,5,"NICK ") == 0)
            commandNick(client , cmd); 
        else if (status == 2 && buff.compare(0,5,"USER ") == 0)
            commandUser(client , cmd);
        else if (buff.compare(0,5,"JOIN ") == 0)
            commandJoin(cmd,client);
        else if (buff.compare(0,5,"KICK ") == 0)
            commandKick(cmd,client);
        else if (buff.compare(0,5,"PART ") == 0)
            commandPart(cmd,client);
        else if (buff.compare(0,8,"PRIVMSG ") == 0)
            commandPrivMsg(cmd,client);
        else if (buff.compare(0,5,"MODE ") == 0)
            commandMode(cmd,client);
        else if (buff.compare(0,7,"INVITE ") == 0)
            commandInvite(cmd,client);
        else if (buff.compare(0,6,"TOPIC ") == 0)
            commandTopic(cmd,client);
        else if (buff.compare(0,7,"NOTICE ") == 0)
            commandNotice(cmd,client);
        else if (buff.size() > 5 && buff.substr(0,5).compare("QUIT ") == 0)
            commandQuit(buff,client);
        else if (buff.size() == 4 && buff.compare("QUIT") == 0)
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

void Server::commandPass(std::map<int,Client>::iterator& it, std::vector<std::string> cmd){
    std::string outputstr("[1] [PASS CORRECT]\n\n");
    std::string parameters("PASS <password>\n");
    std::string parameters_nick("NICK <username>\n");

    if(cmd.size() != 2)
        throw ERR_NEEDMOREPARAMS(PASS);
    if (this->password.compare(cmd[1]) != 0)
    {
        send(it->first, parameters.c_str() ,parameters.size(),0);
        throw ERR_PASSWDMISMATCH();
    }
    else
    {
        it->second.setStatus(1);
        send(it->first,outputstr.c_str(),outputstr.size(),0);
        send(it->first,parameters_nick.c_str(),parameters_nick.size(),0);
    }
}


void Server::commandNick(std::map<int,Client>::iterator& it, std::vector<std::string> cmd){
    std::string outputstr("[2] [NICK NAME SET] : ");
    std::string parameters("NICK <nickname>\n");
    std::string parameters_user("USER <username>\n");

    if(cmd.size() != 2)
        throw ERR_NEEDMOREPARAMS(NICK);
    if(cmd[2].size() == 0)
        throw ERR_NONICKNAMEGIVEN();
    if(compareName(cmd[1], 0))
    {
        send(it->first,parameters.c_str(),parameters.size(),0);
        throw ERR_NICKNAMEINUSE();
    }
    it->second.setNickName(cmd[1]);
    it->second.setStatus(2);
    outputstr.append(it->second.getNick_name());
    outputstr.append("\n\n");
    send(it->first, outputstr.c_str() ,outputstr.size(),0);
    send(it->first,parameters_user.c_str(),parameters_user.size(),0);
}

void Server::commandUser(std::map<int,Client>::iterator& it, std::vector<std::string> cmd){
    std::string outputstr("[3] [USER NAME SET] : ");
    std::string parameters("USER <username>\n");
    
    if(cmd.size() != 2)
        throw ERR_NONICKNAMEGIVEN();
    if(compareName(cmd[1], 1))
    {
        send(it->first,parameters.c_str(),parameters.size(),0);
        throw ERR_NICKNAMEINUSE();
    }
    it->second.setUserName(cmd[1]);
    it->second.setStatus(3);
    outputstr.append(it->second.getUser_name());
    outputstr.append("\n");
    send(it->first,outputstr.c_str(),outputstr.size(),0);
}
