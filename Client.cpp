#include "Client.hpp"

Client::Client(void)
{
    std::cout << "Client Default constructor called" << std::endl;
    return ;
}

Client::Client(Client const & src)
{
    std::cout << "Client Copy constructor called" << std::endl;
    *this = src;
    return ;
}

Client::~Client()
{
    std::cout << "Client Destructor called" << std::endl;
    return ;
}

Client & Client::operator=(Client const & rhs){
    std::cout << " Copy assignment operator called Client" << std::endl;
    (void)rhs;
    return *this;
}


std::ostream & operator<< (std::ostream & o, Client const & rhs){
    (void)rhs;
    return o;
}