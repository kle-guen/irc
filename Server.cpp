#include "Server.hpp"

Server::Server(){}

Server::~Server(){}

const char* Server::ERR_NEEDMOREPARAMS::what() const throw(){
    if(error_code == PASS)
        return("[Error] :PASS <password>\n");
    else if(error_code == NICK)
        return("[Error] :NICK <nickname>\n");
    else if(error_code == USER)
        return("[Error] :USER <username>\n");
    else if(error_code == PRIVMSG)
        return("[Error] :PRIVMSG <receiver> <text to be sent>\n");
    else if(error_code == JOIN)
        return("[Error] :JOIN <channel>{,<channel>} [<key>{,<key>}]\n");
    else if(error_code == MODE)
        return("[Error] :MODE <channel> {[+|-]|o|i|t|l|k} [<limit>] [<user>]\n");
    else if(error_code == KICK)
        return("[Error] :KICK: <channel> <user> [<comment>]\n");
    else if(error_code == PART)
        return("[Error] :PART: <channel>{,<channel>}\n");
    else if(error_code == INVITE)
        return("[Error] :INVITE: <nickname> <channel>\n");
    else if(error_code == TOPIC)
        return("[Error] :TOPIC: <channel> [<topic>]\n");
    else if(error_code == NOTICE)
        return("[Error] :NOTICE: <nickname> <channel>\n");
    else if(error_code == QUIT)
        return("[Error] :QUIT: [<Quit message>]\n");
    return("[Error] :default");
}


const char* Server::ERR_ISNOTOPERATOR::what() const throw(){
    return("[Error] :You are not operator\n");
}

const char* Server::ERR_NOTENOUGHSPACEINCHANNEL::what() const throw(){
    return("[Error] :Not enough space in the channel\n");
}

const char* Server::ERR_CHANNELISFULL::what() const throw(){
    return(m_message.c_str());
}

const char* Server::ERR_NOTEXTTOSEND::what() const throw(){
    return("[Error] :No text to send\n");
}

const char* Server::CommandDoesntExist::what() const throw(){
    return("[Error] :Command doesn't exist\n");
}

const char* Server::ERR_NOSUCHCHANNEL::what() const throw(){
    return(m_message.c_str());
}

const char* Server::RPL_NOTOPIC::what() const throw(){
    return("[Error] :No topic is set\n");
}

const char* Server::ERR_BADCHANNELKEY::what() const throw(){
    return(m_message.c_str());
}

const char* Server::ERR_USERSDONTMATCH::what() const throw(){
    return("[Error] :Client doesn't exist\n");
}

const char* Server::ERR_INVITEONLYCHAN::what() const throw(){
    return(m_message.c_str());
}


const char* Server::ERR_PASSWDMISMATCH::what() const throw(){
    return("[Error] :Password incorrect\n");
}

const char* Server::ERR_USERNOTINCHANNEL::what() const throw(){
    return(m_message.c_str());
}

const char* Server::ERR_NOTONCHANNEL::what() const throw(){
    return(m_message.c_str());
}

const char* Server::ERR_NICKNAMEINUSE::what() const throw(){
    return(m_message.c_str());
}

const char* Server::ERR_NONICKNAMEGIVEN::what() const throw(){
    return("[Error] :No nickname given\n");
}

const char* Server::ERR_ERRONEUSNICKNAME::what() const throw(){
    return(m_message.c_str());
}

const char* Server::ERR_USERNAMEINUSE::what() const throw(){
    return(m_message.c_str());
}

const char* Server::ERR_NOUSERNAMEGIVEN::what() const throw(){
    return("[Error] :No username given\n");
}

const char* Server::ERR_ERRONEUSUSERNAME::what() const throw(){
    return(m_message.c_str());
}

const char* Server::ERR_ALREADYREGISTRED::what() const throw(){
    return("[Error] :You may not reregister\n");
}

const char* Server::ERR_ERRONEUSCHANNEL::what() const throw(){
    return(m_message.c_str());
}

const char* Server::ERR_CANNOTSENDTOCHAN::what() const throw(){
    return m_message.c_str();
}


std::string Server::getPassword(){
    return(password);
}

void Server::setPassword(std::string input_password){
    password = input_password;
}

int Server::parse_nb_client(std::vector<std::string> cmd){
    double tmp;
    char *ptr;

    tmp = strtod(cmd[3].c_str(),&ptr);

    if(ptr[0] != '\0' || tmp > 1000 || tmp < 1)
        throw ERR_NEEDMOREPARAMS(MODE);
    return(std::atoi(cmd[3].c_str()));
}

void    modeInvite(std::string option, std::map<std::string,Channel>::iterator it_channel){
    if(option[0] == 'i' && option.size() == 1)
    {
        if(it_channel->second.getInvite() == 1)
            it_channel->second.setInvite(0);
        else
            it_channel->second.setInvite(1);
    }
    else if(option[1] == 'i' && option[0] == '+')
        it_channel->second.setInvite(true);
    else if(option[1] == 'i' && option[0] == '-')
        it_channel->second.setInvite(false);
}

void    Server::modeTopic(std::string option, std::map<std::string,Channel>::iterator it_channel, std::vector<std::string> cmd){

    if(cmd.size() != 3)
        throw ERR_NEEDMOREPARAMS(MODE);
    if(option[0] == 't' && option.size() == 1)
    {
        if(it_channel->second.getTopic() == 1)
            it_channel->second.setTopic(0);
        else
        {
            it_channel->second.setTopic(1);
        }
    }
    else if(option[1] == 't' && option[0] == '-')
        it_channel->second.setTopic(false);
    else if(option[1] == 't' && option[0] == '+')
        it_channel->second.setTopic(true);
}

void    Server::modeKey(std::string option, std::map<std::string,Channel>::iterator it_channel, std::vector<std::string> cmd){
    if(option[0] == 'k' && option.size() == 1)
    {
        if(it_channel->second.getKey() == 1)
        {
            if(cmd.size() != 3)
                throw ERR_NEEDMOREPARAMS(MODE);
            it_channel->second.setKey(0);
        }
        else
        {
            if(cmd.size() != 4 || cmd[3].size() == 0)
                throw ERR_NEEDMOREPARAMS(MODE);
            it_channel->second.setKey(1);
            it_channel->second.setKeystring(cmd[3]);
        }
    }
    else if(option[1] == 'k' && option[0] == '+')
    {
        if(cmd.size() != 4 || cmd[3].size() == 0)
            throw ERR_NEEDMOREPARAMS(MODE);
        it_channel->second.setKey(1);
        it_channel->second.setKeystring(cmd[3]);
    }
    else if(option[1] == 'k' && option[0] == '-')
        it_channel->second.setKey(0);
}

void    Server::modeOperator(std::string option, std::map<std::string,Channel>::iterator it_channel, std::map<int,Client>::iterator client){
    if((option[0] == 'o' || option[1] == 'o') && it_channel->second.getLimitLen() - 1 < it_channel->second.getNbClient() && it_channel->second.getLimit() == 1)
        throw ERR_NOTENOUGHSPACEINCHANNEL();
    if(option[0] == 'o' && option.size() == 1)
        it_channel->second.invertOperator(client->first);
    else if(option[1] == 'o' && option[0] == '+')
        it_channel->second.addOperator(client->first);
    else if(option[1] == 'o' && option[0] == '-')
        it_channel->second.removeOperator(client->first);
}

void    Server::modeLimit(std::string option, std::map<std::string,Channel>::iterator it_channel, std::vector<std::string> cmd){
    if(option[0] == 'l' && option.size() == 1)
    {
        if(it_channel->second.getLimit() == 1)
        {
            if(cmd.size() != 3)
                throw ERR_NEEDMOREPARAMS(MODE);
            it_channel->second.setLimit(0);
        }
        else
        {
            if(cmd.size() != 4 || cmd[3].size() == 0)
                throw ERR_NEEDMOREPARAMS(MODE);
            if(it_channel->second.getNbClient() > parse_nb_client(cmd))
                throw ERR_NOTENOUGHSPACEINCHANNEL();
            it_channel->second.setLimit(1);
            it_channel->second.setLimitLen(parse_nb_client(cmd));
        }
    }
    else if(option[1] == 'l' && option[0] == '+')
    {
        if(cmd.size() != 4 || cmd[3].size() == 0)
            throw ERR_NEEDMOREPARAMS(MODE);
        if(it_channel->second.getNbClient() > parse_nb_client(cmd))
            throw ERR_NOTENOUGHSPACEINCHANNEL();
        it_channel->second.setLimit(1);
        it_channel->second.setLimitLen(parse_nb_client(cmd));
    }
    else if(option[1] == 'l' && option[0] == '-')
        it_channel->second.setLimit(0);
}

void Server::executemode_channel(std::string option, std::map<std::string,Channel>::iterator it_channel, std::map<int,Client>::iterator client, std::vector<std::string> cmd){
    modeInvite(option,it_channel);
    modeLimit(option,it_channel,cmd);
    modeOperator(option,it_channel,client);
    modeKey(option,it_channel,cmd);
    modeTopic(option,it_channel,cmd);
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

int Server::check_client_existence(std::vector<std::string> target,int src, bool error){
    for(std::vector<std::string>::iterator it_target = target.begin(); it_target != target.end();it_target++)
    {
        if (it_target[0][0] != '#')
        {
            std::map<int, Client>::iterator it_client = this->_server.begin();
            std::map<int, Client>::iterator it_client_end = this->_server.end();
            while(it_client!= it_client_end && it_client->second.getNick_name().compare(*it_target)!=0)
                it_client++;
            if (it_client == it_client_end)
                return(1);
        }
        else
        {   
            std::map<std::string, Channel>::iterator it_channel = this->_vchannel.begin();
            std::map<std::string, Channel>::iterator it_channel_end = this->_vchannel.end();
            while(it_channel!= it_channel_end && it_channel->first != *it_target)
                it_channel++;
            if (it_channel == it_channel_end && error == 1)
                throw ERR_CANNOTSENDTOCHAN(*it_target);
            else if (!it_channel->second.find_client(src) && error == 1)
            {
                throw ERR_CANNOTSENDTOCHAN(*it_target);
            }
        }
    }
    return(0);
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
    for (unsigned long i=0; i< src.size();i++)
    {
        if (src[i]=='_')
            src[i]='-';
        else if (src[i]=='|')
            src[i]='\\';
        else if (src[i]=='{')
            src[i]='[';
        else if (src[i]=='}')
            src[i]=']';
    }
    for (unsigned long i=0; i<src.size();i++)
    {
        if (src[i]!='`' && src[i]!='^' && src[i]!='-' && src[i]!='\\' && src[i]!='[' && src[i]!='[' && !isalpha(src[i]) && !isdigit(src[i]))
        {    
            if (type)
                throw ERR_ERRONEUSUSERNAME(src);
            else
                throw ERR_ERRONEUSNICKNAME(src);
        }
    }
    for(std::map<int, Client>::iterator it = _server.begin();it != _server.end();it++)
    {
        if(type == 0)
            target = it->second.getNick_name();
        else
            target = it->second.getUser_name();
        std::transform(target.begin(),target.end(),target.begin(),::tolower);
        for (unsigned long i=0; i<target.size();i++)
        {
            if (target[i]=='_')
                target[i]='-';
            else if (target[i]=='|')
                target[i]='\\';
            else if (target[i]=='{')
                target[i]='[';
            else if (target[i]=='}')
                target[i]=']';
        }
        if(src.compare(target) == 0)
            return(1);
    }
    return(0);
}

int    Server::compareNameChannel(std::string src){
    std::string target;
    std::transform(src.begin(),src.end(),src.begin(),::tolower);
    
    if (src[0] != '#')
        throw ERR_ERRONEUSCHANNEL(src);
    for (unsigned long i=1; i<src.size();i++)
    {
        if (src[i]!='-' && src[i]!='#' && src[i]!='&' && src[i]!='+' && src[i]!='!' && src[i]!='.' && !isalpha(src[i]) && !isdigit(src[i]))
            throw ERR_ERRONEUSCHANNEL(src);
    }
    return(0);
}


int channel_option(std::string option,int type){
    if((option[type] == 'o' || option[type] == 'i' || option[type] == 't' || option[type] == 'l' 
        || option[type] == 'k'))
        return (1);
    else
        return (0);
}

int check_mode_option(std::string option){
    if(option.size() > 2)
        return(1);
    if(channel_option(option,0) && option.size() == 1)
        return (0);
    if((option[0] == '-' || option[0] == '+') && channel_option(option,1))
        return(0);
    return(1);
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
    int tmp_socket = it->first;

    buffer.reserve(2048);
    len = recv(it->first,buffer_tmp,sizeof(buffer_tmp),0);
    buffer.append(buffer_tmp);
    buffer.erase(len,buffer.size() - len);
    it->second.appendCommand(buffer);
    if (buffer.empty())
    {
        commandQuit(buffer, it, 0);
        return ;
    }
    while(it->second.getCommand().find_first_of('\n') != std::string::npos)
    {
        //do your command
        it->second.eraseBackslash_R();
        tmp = it->second.getCommand().substr(0,it->second.getCommand().find_first_of('\n'));
        tmp[tmp.size()] = 0;

        choose_cmd(tmp, it);

        if(_server.find(tmp_socket) != _server.end())
        {
            it->second.eraseToBackslash_N();
            if(!it->second.getCommand().find_first_of('\n'))
                it->second.resetCommand();
        }
        else
            break;
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
    std::string message_inside("Already inside\n");
    std::map<std::string,Channel>::iterator it;
    if(cmd.size() != 2 && cmd.size() != 3)
        throw ERR_NEEDMOREPARAMS(JOIN);

    parseComma(cmd[1],name_channel);
    if(cmd.size() == 3)
        parseComma(cmd[2],password);
    for(size_t i = 0; i < name_channel.size();i++)
        compareNameChannel(name_channel[i]);

    for(size_t i = 0; i < name_channel.size();i++)
    {
        if(_vchannel.size() == 0)
            channel_exist.push_back(0);
        for(it = _vchannel.begin(); it != _vchannel.end();++it)
        {
            if (name_channel[i].compare(it->first) == 0)
            {
                if(it->second.getInvite() == true)
                    throw ERR_INVITEONLYCHAN(it->first);

                if(_vchannel[name_channel[i]].find_client(client->first) == 1)
                    send(client->first,message_inside.c_str(),message_inside.size(),0);

                if(it->second.getLimit() == 1 && it->second.getLimitLen() - 1 > 0)
                    it->second.setLimitLen(it->second.getLimit() - 1);

                else if(it->second.getLimit() == 1)
                    throw ERR_CHANNELISFULL(it->first);

                if(it->second.getKey() == 0)
                {
                    if(_vchannel[name_channel[i]].find_client(client->first) == 0)
                    {
                        send(client->first,message_add.c_str(),message_add.size(),0);
                        it->second.addClient(client->first);
                    }
                    if(it->second.getTopic() == 1)
                    {
                        if(it->second.getTopicMessage().size() != 0)
                        {
                            send(client->first,it->second.getTopicString().c_str(),it->second.getTopicString().size(),0);
                            send(client->first,"\n",1,0);
                        }
                        else
                            throw RPL_NOTOPIC(it->first);
                    }
                }
                else if(cmd.size() == 3 && cmd[2].size() != 0 && password.size() > i && password[i].compare(it->second.getPassword()) == 0)
                {
                    if(_vchannel[name_channel[i]].find_client(client->first) == 0)
                    {
                        send(client->first,message_add.c_str(),message_add.size(),0);
                        it->second.addClient(client->first);
                    }
                    if(it->second.getTopic() == 1)
                        send(client->first,it->second.getTopicString().c_str(),it->second.getTopicString().size(),0);
                }
                else
                    throw ERR_BADCHANNELKEY(name_channel[i]);
                channel_exist.push_back(1);
                break;
            }
        }
        if(it == _vchannel.end())
            channel_exist.push_back(0);
    }
    for(size_t i = 0; i < name_channel.size();i++)
    {
        if(channel_exist[i] == 0)
        {
            if(cmd.size() != 2 || cmd[1].size() == 0)
                throw ERR_NEEDMOREPARAMS(JOIN);
            send(client->first,message_creat.c_str(),message_creat.size(),0);
            this->_vchannel.insert(std::pair<std::string,Channel>(name_channel[i],instance_channel));
            _vchannel[name_channel[i]].setMode();
            _vchannel[name_channel[i]].addClient(client->first);
        }
    }
}

void Server::commandTopic( std::vector<std::string> cmd,std::map<int,Client>::iterator& client){
    std::map<std::string,Channel>::iterator it;
    std::string message_to_client("Command TOPIC executed\n");
    std::string message_to_no_text("[Error] :Write Topic\n");
    std::string message;

    if(cmd.size() < 2)
        throw ERR_NEEDMOREPARAMS(TOPIC);
    
    it = _vchannel.find(cmd[1]);
    if(it == _vchannel.end())
        throw ERR_NOSUCHCHANNEL(cmd[1]);
    if(it->second.isOperator(client->first) && cmd.size() > 2 && it->second.getTopic() == 1)
    {
        if(cmd[2].size() == 0)
            throw RPL_NOTOPIC(it->first);
        for(size_t i = 2;i < cmd.size();i++)
        {
            message.append(cmd[i]);
            if(i + 1 < cmd.size())
                message.append(" ");
        }
        it->second.setTopicMessage(message);
        send(client->first,message_to_client.c_str(),message_to_client.size(),0);
    }
    else if(cmd.size() == 2)
    {
        if(it->second.getTopicMessage().size() != 0)
        {
            send(client->first,it->second.getTopicString().c_str(),it->second.getTopicString().size(),0);
            send(client->first,"\n",1,0);
        }
        else
            throw RPL_NOTOPIC(it->first);
    }
}

void Server::commandKick(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::string message_to_client("Command KICK executed\n");
    std::map<std::string,Channel>::iterator it;

    if(cmd.size() < 3)
    throw ERR_NEEDMOREPARAMS(KICK);
    
    it = _vchannel.find(cmd[1]);
    if(it == _vchannel.end())
        throw ERR_NOSUCHCHANNEL(cmd[1]);

    if (it->second.find_client(find_socket(cmd[2])->first) == 0)
        throw ERR_USERNOTINCHANNEL(it->first);
    if(it->second.isOperator(client->first) == 1)
    {
        send(client->first,message_to_client.c_str(),message_to_client.size(),0);
        it->second.removeClient(find_socket(cmd[2])->first);
        if(cmd.size() == 4)
        {
            cmd[3].append("\n");
            for(size_t i = 2; i < cmd.size();i++){
                send(find_socket(cmd[2])->first,cmd[i].c_str(),cmd[i].size(),0);
                if(i + 1 < cmd.size())
                    send(find_socket(cmd[2])->first," ",0,0);
            }
        }
    }
    else
        throw ERR_ISNOTOPERATOR();
}

void Server::commandPrivMsg(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::string message_to_client("Command PRIVMSG executed\n");
    std::vector<std::string> target;

    if(cmd.size() < 3 || cmd[2].size() == 0)
        throw ERR_NEEDMOREPARAMS(PRIVMSG);

    parseComma(cmd[1],target);

    cmd[cmd.size()-1].append("\n");

    if(check_client_existence(target,client->first,1) == 1)
        throw ERR_USERSDONTMATCH();

    for(int i = target.size() -1;i != -1; i--)
    {
        if (target[i][0]=='#')
        {
            std::string test;

            for(size_t j = 2; j < cmd.size();j++ )
            {
                test+= cmd[j];
                if(j != cmd.size() - 1)
                    test += " "; 
            }
            this->_vchannel[target[i]].sendMessage(client->second.getUser_name(),client->first,test);
        }   
        else
        {
            for(std::map<int,Client> ::iterator it = _server.begin(); it != _server.end();it++)
            {
                if (it->second.getNick_name().compare(target[i]) == 0)
                {
                    send(client->first,message_to_client.c_str(),message_to_client.size(),0);
                    send(it->first,client->second.getNick_name().c_str(),client->second.getNick_name().size(),0);
                    send(it->first," :",2,0);
                    for(size_t j = 2; j < cmd.size();j++ )
                    {
                        send(it->first,cmd[j].c_str(),cmd[j].size(),0);
                        if(j + 1 < cmd.size())
                            send(it->first," ",1,0);
                    }
                }
            }
        }
    }

}

void Server::commandMode(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::string message_to_client("Command MODE ");
    std::map<std::string,Channel>::iterator it;
    std::map<int,Client> ::iterator id;
    
    (void)client;
    if(cmd.size() < 3 || cmd.size() > 4)
    {
        _type_error = 1;
        throw ERR_NEEDMOREPARAMS(MODE);
    }

    it = _vchannel.find(cmd[1]);

    if(it == _vchannel.end())
        throw ERR_NOSUCHCHANNEL(cmd[1]);

    if(!it->second.isOperator(client->first))
        throw ERR_ISNOTOPERATOR();
    if(check_mode_option(cmd[2]))
        throw ERR_NEEDMOREPARAMS(MODE);
    if(cmd[2][0] == 'o' || cmd[2][1] == 'o')
    {
        if(cmd.size() != 4 || cmd[3].size() == 0)
            throw ERR_NEEDMOREPARAMS(MODE);
        id = find_socket(cmd[3]);
        if(!it->second.find_client(id->first))
            throw ERR_USERNOTINCHANNEL(it->first);
    }
    executemode_channel(cmd[2], it, id, cmd);
    message_to_client.append(cmd[2] + " executed\n");
    send(client->first,message_to_client.c_str(),message_to_client.size(),0);
}

void Server::commandPart(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::string message_to_client("Command PART executed\n");

    std::map<std::string,Channel>::iterator it;
    std::vector<std::string> name_channel;

    if(cmd.size() != 2)
        throw ERR_NEEDMOREPARAMS(PART);

    parseComma(cmd[1],name_channel);
    for(size_t i = 0; i < name_channel.size();i++)
    {
        it = _vchannel.find(name_channel[i]);
        if(it == _vchannel.end())
            throw ERR_NOSUCHCHANNEL(name_channel[i]);
        if(!it->second.find_client(client->first))
            throw ERR_NOTONCHANNEL(it->first);
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
        throw ERR_NOSUCHCHANNEL(cmd[2]);

    if (client->first == it->second.getOperator() )
    {
        if(it->second.getLimitLen() - 1 < it->second.getNbClient() && it->second.getLimit() == 1)
            throw ERR_CHANNELISFULL(it->first);
        it->second.addClient(id->first);
        send(client->first,message_to_client.c_str(),message_to_client.size(),0);
    }
}

void Server::commandNotice(std::vector<std::string> cmd,std::map<int,Client>::iterator client){
    std::string message_to_client("Command NOTICE executed\n");
    std::vector<std::string> target;

    if(cmd.size() < 3 || cmd[2].size() == 0)
        return ;

    parseComma(cmd[1],target);

    cmd[cmd.size()-1].append("\n");

    if(check_client_existence(target,client->first,0) == 1)
        return ;
    for(int i = target.size() -1;i != -1; i--)
    {
        if (target[i][0]=='#')
        {
            std::string test;

            for(size_t j = 2; j < cmd.size();j++ )
            {
                test+= cmd[j];
                if(j != cmd.size() - 1)
                    test += " "; 
            }
            this->_vchannel[target[i]].sendMessage(client->second.getUser_name(),client->first,test);
        }   
        else
        {
            for(std::map<int,Client> ::iterator it = _server.begin(); it != _server.end();it++)
            {
                if (it->second.getNick_name().compare(target[i]) == 0)
                {
                    send(client->first,message_to_client.c_str(),message_to_client.size(),0);
                    send(it->first,client->second.getNick_name().c_str(),client->second.getNick_name().size(),0);
                    send(it->first," :",2,0);
                    for(size_t j = 2; j < cmd.size();j++ )
                    {
                        send(it->first,cmd[j].c_str(),cmd[j].size(),0);
                        if(j + 1 < cmd.size())
                            send(it->first," ",1,0);
                    }
                }
            }
        }
    }
}

void Server::commandQuit(std::string buff,std::map<int,Client>::iterator client, int type){
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
        else
            ++it_c;
    }
    if (buff.size() == 4 && type)
    {
        out.append(client->second.getNick_name() + " left\n");
        sendFromClient(client, out);
    }
    else if (type)
    {
        out = buff.substr(buff.find(' ') + 1, buff.size());
        out.append("\n");
        sendFromClient(client, out);
    }
    send(client->first,message_to_client.c_str(),message_to_client.size(),0);
    close(client->first);
    _server.erase(client->first);
}

void Server::commandPrint(std::map<int,Client>::iterator client){
    std::map<int, Client> tmp = _server;
    char buffer[2048];
    std::string client_names = "Client name in the server: ";
    send(client->first,client_names.c_str(),client_names.size(),0);
    for(std::map<int, Client>::iterator it_server = _server.begin();it_server != _server.end();++it_server)
    {
        std::string outclient_names = it_server->second.getNick_name() + " ";
        send(client->first,outclient_names.c_str(),outclient_names.size(),0);
    }
    send(client->first,"\n",1,0);
    for(std::map<std::string, Channel>::iterator it =_vchannel.begin() ; it != _vchannel.end();++it)
    {
        std::string channel_name = "Channel name: " + it->first;
        std::string mode_t = " t: ";
        std::sprintf(buffer, "%d", it->second.getTopic());
        mode_t.append(buffer);
        std::string mode_i = " i: ";
        std::sprintf(buffer, "%d", it->second.getInvite());
        mode_i.append(buffer);
        std::string mode_k = " k: ";
        std::sprintf(buffer, "%d", it->second.getKey());
        mode_k.append(buffer);
        std::string mode_Topic = " t: " + it->second.getTopicMessage();
        std::string mode_NoTopic = " t: 0";
        std::string mode_Len = " l: ";
        std::sprintf(buffer, "%d", it->second.getLimitLen());
        mode_Len.append(buffer);
        std::string mode_Key = " key: " + password;
        std::string mode_Nboperator = " o: ";
        std::sprintf(buffer, "%d", it->second.getNbOperator());
        mode_Nboperator.append(buffer);


        send(client->first,"\n",1,0);
        std::deque<int> tmp_channel = it->second.getClientBase();
        
        send(client->first,channel_name.c_str(),channel_name.size(),0);

        if (it->second.getTopic() == 1)
        {
            send(client->first,mode_Topic.c_str() ,mode_Topic.size() - 1,0);
        }
        else
            send(client->first,mode_NoTopic.c_str() ,mode_NoTopic.size(),0);

            
        send(client->first,mode_i.c_str(),mode_i.size(),0);

        send(client->first,mode_Nboperator.c_str(),mode_Nboperator.size(),0);

        if (it->second.getLimit() == 1)
            send(client->first,mode_Len.c_str(),mode_Len.size(),0);
        else
            send(client->first," l: No limit ",13,0);
        if (it->second.getKey() == 1)
        {
            send(client->first,mode_k.c_str(),mode_k.size(),0);
            if(it->second.isOperator(client->first))
                send(client->first,mode_Key.c_str(),mode_Key.size(),0);
        }
        else
            send(client->first," k: No Key ",11,0);
        send(client->first,"\n",1,0);
        std::deque<int> _client_base;
        send(client->first,"Client in the channel: ",23,0);
        for(std::deque<int>::iterator it_client = tmp_channel.begin(); it_client != tmp_channel.end(); it_client++)
        {
            std::string client_name = _server[*it_client].getNick_name() + " ";
            send(client->first,client_name.c_str(),client_name.size(),0);
        }
        send(client->first,"\n",1,0);
    }
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
        if (buff.compare(0,4,"PASS") == 0)
            commandPass(client , cmd, status);
        else if ((status != 0 && status != 2) && buff.compare(0,4,"NICK") == 0)
            commandNick(client , cmd, status);
        else if ((status != 0 && status != 1) && buff.compare(0,4,"USER") == 0)
            commandUser(client , cmd, status);
        else if (status == 3 && buff.compare(0,4,"JOIN") == 0)
            commandJoin(cmd,client);
        else if (status == 3 && buff.compare(0,4,"KICK") == 0)
            commandKick(cmd,client);
        else if (status == 3 && buff.compare(0,4,"PART") == 0)
            commandPart(cmd,client);
        else if (status == 3 && buff.compare(0,7,"PRIVMSG") == 0)
            commandPrivMsg(cmd,client);
        else if (status == 3 && buff.compare(0,4,"MODE") == 0)
            commandMode(cmd,client);
        else if (status == 3 && buff.compare(0,6,"INVITE") == 0)
            commandInvite(cmd,client);
        else if (status == 3 && buff.compare(0,5,"TOPIC") == 0)
            commandTopic(cmd,client);
        else if (status == 3 && buff.compare(0,6,"NOTICE") == 0)
            commandNotice(cmd,client);
        else if (status == 3 && buff.compare(0,5,"PRINT") == 0)
            commandPrint(client);
        else if (status == 3 && buff.size() > 4 && buff.substr(0,5).compare("QUIT") == 0)
            commandQuit(buff,client, 1);
        else if (status == 3 && buff.size() == 4 && buff.compare("QUIT") == 0)
            commandQuit(buff,client, 1);
        else if (status == 0)
            throw ERR_NEEDMOREPARAMS(PASS);
        else if (status == 1)
            throw ERR_NEEDMOREPARAMS(NICK);
        else if (status == 2)
            throw ERR_NEEDMOREPARAMS(USER); 
        else
            throw CommandDoesntExist();
    }
    catch(const std::exception& e)
    {
        outerror.append(e.what());
        send(client->first,outerror.c_str(),outerror.size(),0);
    }
    
}

void Server::commandPass(std::map<int,Client>::iterator& it, std::vector<std::string> cmd, int status){
    std::string outputstr("[1] [PASS CORRECT]\n\n");
    std::string parameters_nick("NICK <nickname>\n");

    if (status != 0)
        throw ERR_ALREADYREGISTRED();
    if(cmd.size() != 2)
        throw ERR_NEEDMOREPARAMS(PASS);
    if (this->password.compare(cmd[1]) != 0)
        throw ERR_PASSWDMISMATCH();
    else
    {
        it->second.setStatus(1);
        send(it->first,outputstr.c_str(),outputstr.size(),0);
        send(it->first,parameters_nick.c_str(),parameters_nick.size(),0);
    }
}

void Server::commandNick(std::map<int,Client>::iterator& it, std::vector<std::string> cmd, int status){
    std::string outputstr1("[2] [NICK NAME SET] : ");
    std::string outputstr2("Nickname changed to : ");
    std::string parameters_user("USER <username>\n");

    if(cmd.size() != 2)
        throw ERR_NEEDMOREPARAMS(NICK);
    if(cmd[1].size() == 0)
        throw ERR_NONICKNAMEGIVEN();
    if(cmd[1].size() > 9)
        throw ERR_ERRONEUSNICKNAME(cmd[1]);
    if(compareName(cmd[1], 0))
        throw ERR_NICKNAMEINUSE(cmd[1]);
    it->second.setNickName(cmd[1]);
    if (status == 1)
    {
        it->second.setStatus(2);
        outputstr1.append(it->second.getNick_name());
        outputstr1.append("\n\n");
        send(it->first, outputstr1.c_str() ,outputstr1.size(),0);
        send(it->first,parameters_user.c_str(),parameters_user.size(),0);
    }
    else
    {
        outputstr2.append(it->second.getNick_name());
        outputstr2.append("\n");
        send(it->first, outputstr2.c_str() ,outputstr2.size(),0);
    }
}

void Server::commandUser(std::map<int,Client>::iterator& it, std::vector<std::string> cmd, int status){
    std::string outputstr("[3] [USER NAME SET] : ");

    if (status == 3)
        throw ERR_ALREADYREGISTRED();
    if(cmd.size() < 2)
        throw ERR_NEEDMOREPARAMS(USER);
    if(cmd[1].size() == 0)
        throw ERR_NOUSERNAMEGIVEN();
    if(cmd[1].size() > 9)
        throw ERR_ERRONEUSUSERNAME(cmd[1]);
    if(compareName(cmd[1], 1))
        throw ERR_USERNAMEINUSE(cmd[1]);
    it->second.setUserName(cmd[1]);
    it->second.setStatus(3);
    outputstr.append(it->second.getUser_name());
    outputstr.append("\n");
    send(it->first,outputstr.c_str(),outputstr.size(),0);
}
