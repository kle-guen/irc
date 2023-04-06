#ifndef Client_HPP
#define Client_HPP

#include <iostream>
#include <string>

class Client
{
private:
    std::string nick_name;
    int status;
protected:
public:
    //format canonique
    Client(void);
    Client(Client const & src);
    Client & operator=(Client const & rhs);
    ~Client();

    //additional constructor
   
    //getter
    int getStatus(){
        return(status);
    }
    //functions

};
std::ostream & operator<<(std::ostream & o, Client const & rhs);

#endif