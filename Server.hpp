#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <vector>
#include <map>
#include <string> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <algorithm>
#include <cstdio>
#include <cstring>
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
    ~Server();

    void initServer(char **av);
    void message_receiver(std::map<int, Client>::iterator it);
    void erase_map_element(std::map<int, Client>::iterator &it);
    int check_channel_exist(std::string chanel, std::map<int, Client>::iterator client);
    void parseComma(std::string buff, std::vector<std::string> &target);
    void executemode_user(std::string option, std::map<std::string,Channel>::iterator it, std::map<int,Client>::iterator client);
    int find_socket_channel(int id,std::deque<int> _vchannel);
    int compareName(std::string target,int type);
    int  compareNameChannel(std::string src);
    
    void sendFromClient(std::map<int,Client>::iterator client, std::string message);
    int check_client_existence(std::vector<std::string> target,int src,bool error);

    void choose_cmd(std::string buff, std::map<int, Client>::iterator it);
    void commandPass(std::map<int, Client>::iterator &it, std::vector<std::string> cmd, int status);
    void commandNick(std::map<int, Client>::iterator &it, std::vector<std::string> cmd, int status);
    void commandUser(std::map<int, Client>::iterator &it, std::vector<std::string> cmd, int status);
    void commandJoin(std::vector<std::string> cmd, std::map<int, Client>::iterator it);
    void commandSend(std::vector<std::string> cmd, std::map<int, Client>::iterator it);
    void commandPrivMsg(std::vector<std::string> cmd, std::map<int, Client>::iterator it);
    void commandKick(std::vector<std::string> cmd, std::map<int, Client>::iterator client);
    void commandMode(std::vector<std::string> cmd, std::map<int, Client>::iterator client);
    void commandInvite(std::vector<std::string> cmd, std::map<int, Client>::iterator client);
    void commandPart(std::vector<std::string> cmd, std::map<int, Client>::iterator client);
    void commandNotice(std::vector<std::string> cmd,std::map<int,Client>::iterator client);
    void commandQuit(std::string buff,std::map<int,Client>::iterator client, int type);
    void commandTopic( std::vector<std::string> cmd,std::map<int,Client>::iterator& client);
    void commandPrint(std::map<int,Client>::iterator client);
    void executemode_channel(std::string option, std::map<std::string,Channel>::iterator it_channel, std::map<int,Client>::iterator client, std::vector<std::string> cmd);
    int parse_nb_client(std::vector<std::string> cmd);
    void modeLimit(std::string option, std::map<std::string,Channel>::iterator it_channel, std::vector<std::string> cmd);
    void modeKey(std::string option, std::map<std::string,Channel>::iterator it_channel, std::vector<std::string> cmd);
    void modeOperator(std::string option, std::map<std::string,Channel>::iterator it_channel, std::map<int,Client>::iterator client);


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
    // class ERR_CANNOTSENDTOCHAN : public std::exception
    // {
    // public:
    //     std::string _name;
    //     ERR_CANNOTSENDTOCHAN(std::string name): _name(name) {}
    //     virtual const char *what() const throw();
    //     virtual ~ERR_CANNOTSENDTOCHAN()throw(){};
    // };
    class ERR_CANNOTSENDTOCHAN : public std::exception
    {
        public:
            ERR_CANNOTSENDTOCHAN(const std::string& message) throw() : m_message(message + ": Cannot send to channel\n") {}
            ~ERR_CANNOTSENDTOCHAN() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
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
            ERR_NOSUCHCHANNEL(const std::string& message) throw() : m_message(message + ":No such channel\n") {}
            ~ERR_NOSUCHCHANNEL() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };
    class ERR_PASSWDMISMATCH : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

    class ERR_USERNOTINCHANNEL : public std::exception
    {
        public:
            ERR_USERNOTINCHANNEL(const std::string& message) throw() : m_message(message + ":They aren't on that channel\n") {}
            ~ERR_USERNOTINCHANNEL() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };
    class ERR_NOTONCHANNEL : public std::exception
    {
        public:
            ERR_NOTONCHANNEL(const std::string& message) throw() : m_message(message + ":You're not on that channel\n") {}
            ~ERR_NOTONCHANNEL() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };
    class ERR_BADCHANNELKEY : public std::exception
    {
        public:
            ERR_BADCHANNELKEY(const std::string& message) throw() : m_message(message + ":Cannot join channel\n") {}
            ~ERR_BADCHANNELKEY() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };
    class ERR_NOTEXTTOSEND : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };

    class ERR_NICKNAMEINUSE : public std::exception
    {
        public:
            ERR_NICKNAMEINUSE(const std::string& message) throw() : m_message(message + ":Nickname is already in use\n") {}
            ~ERR_NICKNAMEINUSE() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };
    class ERR_NONICKNAMEGIVEN : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_ERRONEUSNICKNAME : public std::exception
    {
        public:
            ERR_ERRONEUSNICKNAME(const std::string& message) throw() : m_message(message + ":Erroneus nickname\n") {}
            ~ERR_ERRONEUSNICKNAME() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };
    
    class ERR_USERNAMEINUSE : public std::exception
    {
        public:
            ERR_USERNAMEINUSE(const std::string& message) throw() : m_message(message + ":Username is already in use\n") {}
            ~ERR_USERNAMEINUSE() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };
    class ERR_NOUSERNAMEGIVEN : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_ERRONEUSUSERNAME : public std::exception
    {
        public:
            ERR_ERRONEUSUSERNAME(const std::string& message) throw() : m_message(message + ":Erroneus username\n") {}
            ~ERR_ERRONEUSUSERNAME() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };
    class ERR_ERRONEUSCHANNEL : public std::exception
    {
        public:
            ERR_ERRONEUSCHANNEL(const std::string& message) throw() : m_message(message + ":Erroneus channel name\n") {}
            ~ERR_ERRONEUSCHANNEL() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };
    class ERR_ALREADYREGISTRED : public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_INVITEONLYCHAN: public std::exception
    {
        public:
            ERR_INVITEONLYCHAN(const std::string& message) throw() : m_message(message + ":Cannot join channel \n") {}
            ~ERR_INVITEONLYCHAN() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };
    
    class ERR_ISNOTOPERATOR: public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class ERR_CHANNELISFULL: public std::exception
    {
        public:
            ERR_CHANNELISFULL(const std::string& message) throw() : m_message(message + ":Cannot join channel \n") {}
            ~ERR_CHANNELISFULL() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };
    class ERR_NOTENOUGHSPACEINCHANNEL: public std::exception
    {
    public:
        virtual const char *what() const throw();
    };
    class RPL_NOTOPIC: public std::exception
    {
        public:
            RPL_NOTOPIC(const std::string& message) throw() : m_message(message + ":No topic is set\n") {}
            ~RPL_NOTOPIC() throw() {}
            const char* what() const throw() ;
        private:
            std::string m_message;
    };

};

#endif