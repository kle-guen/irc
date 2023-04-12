#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <iostream>
# include "Client.hpp"
# include <map>
# include <deque>
# include <string>
# include <sys/socket.h>

typedef struct s_mode
{
    bool i;
}   t_mode;

class Channel
{
    public:
        Channel();
        Channel(Channel const & src);
        Channel(int id,std::string name);
        Channel & operator=(Channel const & rhs);
        void addClient(int id);
        void sendMessage(std::string name, int id, std::string message);
        void setName(std::string name);
        std::string getName() const;
        void removeClient(int id,int id_remover);
        void setPassword(std::string password);
        int getOperator();
        void changeOperator(int old_operator,int new_operator);
        void setInvite(int type);
        bool getInvite();
        std::string getPassword() const;

        ~Channel();
    private:
        std::string _name;
        std::string _password;
        t_mode _mode;
        std::deque<int> _client_base;
};

#endif