#include "Channel.hpp"

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

Channel::~Channel()
{

}