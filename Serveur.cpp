#include "Serveur.hpp"

Serveur::Serveur(void)
{
    std::cout << "Serveur Default constructor called" << std::endl;
    return ;
}

Serveur::Serveur(Serveur const & src)
{
    std::cout << "Serveur Copy constructor called" << std::endl;
    *this = src;
    return ;
}

Serveur::~Serveur()
{
    std::cout << "Serveur Destructor called" << std::endl;
    return ;
}

Serveur & Serveur::operator=(Serveur const & rhs){
    std::cout << " Copy assignment operator called Serveur" << std::endl;
    (void)rhs;
    return *this;
}


std::ostream & operator<< (std::ostream & o, Serveur const & rhs){
    (void)rhs;
    return o;
}