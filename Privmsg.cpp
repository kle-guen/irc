#include "Privmsg.hpp"

Privmsg::Privmsg() : ACommand("PRIVMSG ")
{

}

Privmsg::Privmsg(Privmsg const & src) : ACommand(src)
{
    *this = src;
}

Privmsg &Privmsg::operator=(Privmsg const & rhs)
{
    (void)rhs;
    return (*this);
}

Privmsg::~Privmsg()
{

}

void    Privmsg::execCommand(void)
{
    std::cout << "Privmsg" << std::endl;
}
