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
    bool t;
    bool k;
    bool l;
    int limit;
    int o;
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
        std::string getName() const;
        void removeClient(int id);
        int isOperator(int id);
        int  find_client(int id);

        
        int getNbOperator();
        int getNbClient();
        int getOperator();
        bool getTopic();
        std::string getTopicString();
        bool getInvite();
        int getLimitLen();
        bool getLimit();
        bool getKey();

        void setName(std::string name);
        void setPassword(std::string password);
        void setMode();
        void setKey(bool stat);
        void setKeystring(std::string str);
        void setInvite(int type);
        void setLimitLen(int type);
        void setLimit(bool stat);
        void setTopic(bool stat);
        void setTopicMessage(std::string message);

        void changeOperator(int old_operator,int new_operator);
        void removeOperator(int new_operator);
        void addOperator(int new_operator);
        void invertOperator(int new_operator);
        
        std::deque<int> getClientBase();
        std::string getPassword() const;
        std::string getTopicMessage();

        ~Channel();
    private:
        std::string _name;
        std::string _password;
        std::string _topic;
        t_mode _mode;
        std::deque<int> _client_base;
        std::deque<int> _client_black_list;
        std::deque<int> _mode_white_list;
};

#endif