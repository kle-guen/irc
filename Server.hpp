#ifndef SERVER_HPP
# define SERVER_HPP
# include <iostream>
# include <vector>
# include <map>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <arpa/inet.h>

# include "Client.hpp"
# include "Channel.hpp"
# include "ACommand.hpp"
# include "Join.hpp"

# define MAX_CLIENTS 10
# define GREEN  "\e[32m"
# define RED  "\e[31m"
# define RESET  "\e[0m"

class Server
{
    public:

        Server();
        Server(Server const & src);
        Server & operator=(Server const & rhs);
        ~Server();

        void initServer(char **av);
        void message_receiver(std::map<int,Client>::iterator it, std::string password);
        void erase_map_element(std::map<int,Client>::iterator& it);
        void check_password(std::map<int,Client>::iterator& it, std::string password, std::string buffer);
        void check_nick_name(std::map<int,Client>::iterator& it, std::string buffer);
        void check_user_name(std::map<int,Client>::iterator& it, std::string buffer);
        
        void choose_cmd(std::string buff,std::map<int,Client>::iterator it);
        void commandSend(std::string buff,std::map<int,Client>::iterator it);
        void commandPrivMsg(std::string buff,std::map<int,Client>::iterator it);
        void commandJoin(std::string buff,std::map<int,Client>::iterator it);

    private:

        std::map<int,Client> _server;
        std::map<std::string,Channel> _vchannel;
};

#endif