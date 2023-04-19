#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include <map>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <algorithm>

#include "Client.hpp"
#include "Channel.hpp"

#define MAX_CLIENTS 10
#define GREEN "\e[32m"
#define RED "\e[31m"
#define RESET "\e[0m"
#define USERMODE 1
#define CHANNELMODE 2

enum{PASS,NICK,USER,JOIN,KICK,NOTICE,PRIVMSG,MODE,TOPIC,QUIT,PART,INVITE};


class Server
{
public:
    Server();
    Server(Server const &src);
    Server &operator=(Server const &rhs);
    ~Server();

    void initServer(char **av);
    void message_receiver(std::map<int, Client>::iterator it);
    void erase_map_element(std::map<int, Client>::iterator &it);
    int check_channel_exist(std::string chanel, std::map<int, Client>::iterator client);
    void parseComma(std::string buff, std::vector<std::string> &target);
    void executemode_user(std::string option, std::map<std::string,Channel>::iterator it, std::map<int,Client>::iterator client);
    int find_socket_channel(int id,std::deque<int> _vchannel);
    int compareName(std::string target,int type,std::map<int,Client>::iterator& it);
    void sendFromClient(std::map<int,Client>::iterator client, std::string message);
    int check_client_existence(std::vector<std::string> target,int src);

    void choose_cmd(std::string buff, std::map<int, Client>::iterator it);
    void commandPass(std::map<int, Client>::iterator &it, std::vector<std::string> cmd);
    void commandNick(std::map<int, Client>::iterator &it, std::vector<std::string> cmd);
    void commandUser(std::map<int, Client>::iterator &it, std::vector<std::string> cmd);
    void commandJoin(std::vector<std::string> cmd, std::map<int, Client>::iterator it);
    void commandSend(std::vector<std::string> cmd, std::map<int, Client>::iterator it);
    void commandPrivMsg(std::vector<std::string> cmd, std::map<int, Client>::iterator it);
    void commandKick(std::vector<std::string> cmd, std::map<int, Client>::iterator client);
    void commandMode(std::vector<std::string> cmd, std::map<int, Client>::iterator client);
    void commandInvite(std::vector<std::string> cmd, std::map<int, Client>::iterator client);
    void commandPart(std::vector<std::string> cmd, std::map<int, Client>::iterator client);
    void commandNotice(std::vector<std::string> cmd,std::map<int,Client>::iterator client);
    void commandQuit(std::string buff,std::map<int,Client>::iterator client);
    void commandTopic( std::vector<std::string> cmd,std::map<int,Client>::iterator& client);
    void executemode_channel(std::string option, std::map<std::string,Channel>::iterator it_channel, std::map<int,Client>::iterator client, std::vector<std::string> cmd);
    int parse_nb_client(std::vector<std::string> cmd);
    void modeLimit(std::string option, std::map<std::string,Channel>::iterator it_channel, std::vector<std::string> cmd);
    void modeKey(std::string option, std::map<std::string,Channel>::iterator it_channel, std::vector<std::string> cmd);



    int getTypeError(void){return(_type_error);}
    std::string getPassword();
    void setPassword(std::string input_password);
    std::map<int, Client>::iterator find_socket(std::string target);
    std::map<std::string, Channel> getVchannel() const;

private:
    int _type_error;
    std::map<int, Client> _server;
    std::map<std::string, Channel> _vchannel;
    std::string password;
    std::vector<std::string> uper_client;
    
    class ERR_CANNOTSENDTOCHAN : public std::exception
    {
    public:
        std::string _name;
        ERR_CANNOTSENDTOCHAN(std::string name): _name(name) {}
        virtual const char *what() const throw();
        virtual ~ERR_CANNOTSENDTOCHAN()throw(){};
    };
    class ERR_NEEDMOREPARAMS : public std::exception
    {
    public:
        int error_code;
        ERR_NEEDMOREPARAMS(int code) : error_code(code) {}
        virtual const char *what() const throw();
    };
    class CommandDoesntExist : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_USERSDONTMATCH : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_NOSUCHCHANNEL : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_PASSWDMISMATCH : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_NOTONCHANNEL : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_BADCHANNELKEY : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_NOTEXTTOSEND : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

    class ERR_NICKNAMEINUSE : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_NONICKNAMEGIVEN : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_ERRONEUSNICKNAME : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

    class ERR_ISNOTOPERATOR: public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_CHANNELISFULL: public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_NOTENOUGHSPACEINCHANNEL: public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
};

#endif