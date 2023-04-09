#include "Channel.hpp"
#include <algorithm>
#include <vector>
Channel::Channel()
{

}

Channel::Channel(Channel const & src)
{
    *this = src;
}

Channel &Channel::operator=(Channel const & rhs)
{
    (void)rhs;
    return (*this);
}

Channel::Channel(int id, std::string name)
{
    std::cout<<"Chanel Constructor"<<std::endl;
    this->_client_base.push_back(id);
    this->_name = name;
}

Channel::~Channel()
{

}

void Channel::sendMessage(std::string name, int id, std::string message)
{
    std::deque<int>::iterator it = this->_client_base.begin();
    if (std::find(this->_client_base.begin(),this->_client_base.end(),id) == this->_client_base.end())
        return;
    while(it!= _client_base.end())
    {
        send(*it,name.c_str(),name.size(),0);
        send(*it," :",2,0);
        send(*it,message.c_str(),message.size(),0);
        it++;
    }
}

void Channel::removeClient(int id,int id_remover)
{
    std::deque<int>::iterator it = this->_client_base.begin();
    if (id_remover != *it)
        return ;
    while(it!= this->_client_base.end())
    {
        if (id == *it)
        {
            this->_client_base.erase(it);
            return;
        }
        it++;
    }
}

std::string Channel::getName() const{
    return(_name);
}

std::string Channel::getPassword() const{
    return(_password);
}

void Channel::addClient(int id)
{
    std::deque<int>::iterator it = this->_client_base.begin();
    while(it!= _client_base.end())
    {
        if (id == *it)
        {
            send(id,"Already Inside :",16,0);
            send(id,this->_name.c_str(),this->_name.size(),0);
            send(id,"\n",1,0);
            return;
        }
        it++;
    }
    this->_client_base.push_back(id);
}

void Channel::setPassword(std::string password){
    _password = password;
}