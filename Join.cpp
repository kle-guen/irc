#include "Join.hpp"

Join::Join() : ACommand("JOIN ")
{

}

Join::Join(Join const & src) : ACommand(src)
{
    *this = src;
}

Join &Join::operator=(Join const & rhs)
{
    (void)rhs;
    return (*this);
}

Join::~Join()
{

}

void    Join::execCommand(void)
{
    std::cout << "join" << std::endl;
}
