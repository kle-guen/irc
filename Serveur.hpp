#ifndef Serveur_HPP
#define Serveur_HPP

#include <iostream>
#include "Client.hpp"
#include <string>
#include <map>

class Serveur
{
private:
protected:
public:
    //format canonique
    Serveur(void);
    Serveur(Serveur const & src);
    Serveur & operator=(Serveur const & rhs);
    ~Serveur();

    //additional constructor
   
    //getter

    //functions

};
std::ostream & operator<<(std::ostream & o, Serveur const & rhs);

#endif