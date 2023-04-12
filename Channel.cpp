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
    _mode.i = false;
    _mode.o =1;
    std::cout<<"Chanel Constructor"<<std::endl;
    this->_client_base.push_back(id);
    this->_name = name;
}

Channel::~Channel()
{

}

int Channel::getOperator()
{
    return(*this->_client_base.begin());
}


/*
void Channel::removeOperator(int old_operator,int new_operator){
    (void)old_operator;
    (void)new_operator;
}

void Channel::addOperator(int old_operator,int new_operator){
    (void)old_operator;
    (void)new_operator;
}

void Channel::invertOperator(int old_operator,int new_operator){
    (void)old_operator;
    (void)new_operator;
}
*/
void Channel::changeOperator(int old_operator,int new_operator)
{
    if (old_operator == this->getOperator() && std::find(this->_client_base.begin(),this->_client_base.end(),new_operator) != this->_client_base.end())
    {
        std::deque<int>::iterator it = std::find(this->_client_base.begin(),this->_client_base.end(),new_operator);
        this->_client_base.push_front(*it);
        this->_client_base.erase(it);
    }
}

int Channel::isOperator(int id)
{
    if (std::find(this->_client_base.begin(),this->_client_base.begin()+this->_mode.o,id) == this->_client_base.begin()+this->_mode.o)
        return(0);
    return(1);
}

void Channel::removeOperator(int new_operator)
{
    if (std::find(this->_client_base.begin(),this->_client_base.end(),new_operator) == this->_client_base.end())
    {
        return;
    }
    else if (std::find(this->_client_base.begin(),this->_client_base.begin()+this->_mode.o,new_operator) != this->_client_base.begin()+this->_mode.o)
    {
        std::deque<int>::iterator it = std::find(this->_client_base.begin(),this->_client_base.end(),new_operator);
        this->_client_base.push_back(*it);
        this->_client_base.erase(it);
        this->_mode.o--;
    }
}

int  Channel::find_client(int id){
    std::deque<int>::iterator it;

    for(it = _client_base.begin();it != _client_base.end();it++)
        if(*it == id)
            return(1);
    return(0);
}

int Channel::getNbOperator()
{
    return(this->_mode.o);
}

void Channel::addOperator(int new_operator)
{
    if (std::find(this->_client_base.begin(),this->_client_base.end(),new_operator) == this->_client_base.end())
    {
        this->_client_base.push_front(new_operator);
        this->_mode.o++;
    }
    else if (std::find(this->_client_base.begin(),this->_client_base.begin()+this->_mode.o,new_operator) == this->_client_base.begin()+this->_mode.o)
    {
        std::deque<int>::iterator it = std::find(this->_client_base.begin(),this->_client_base.end(),new_operator);
        this->_client_base.push_front(*it);
        this->_client_base.erase(it);
        this->_mode.o++;
    }
}

void Channel::invertOperator(int new_operator)
{
    if (std::find(this->_client_base.begin(),this->_client_base.end(),new_operator) != this->_client_base.end())
    {
        if (std::find(this->_client_base.begin(),this->_client_base.begin()+this->_mode.o,new_operator) == this->_client_base.begin()+this->_mode.o)
        {
            std::deque<int>::iterator it = std::find(this->_client_base.begin(),this->_client_base.end(),new_operator);
            this->_client_base.push_front(*it);
            this->_client_base.erase(it);
            this->_mode.o++;
        }
        else
        {
            std::deque<int>::iterator it = std::find(this->_client_base.begin(),this->_client_base.end(),new_operator);
            this->_client_base.push_back(*it);
            this->_client_base.erase(it);
            this->_mode.o--;
        }
    }
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

void Channel::removeClient(int id)
{
    std::deque<int>::iterator it = this->_client_base.begin();
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

void Channel::setInvite(int type){
    _mode.i = type;
}

bool Channel::getInvite(){
    return(_mode.i);
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