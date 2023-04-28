#include "Server.hpp"

int main(int ac, char **av)
{
    if(ac != 3)
    {
        std::cout << "./irc [port] [password]" << std::endl;
        return(1);
    }
    std::string password(av[2]);
    std::string port(av[1]);
    if( port.size() != 4 || port.compare("0000") == 0)
    {
        std::cout << "[Error]: wrong port" << std::endl;
        return(1);
    }
    for(size_t i = 0;i < port.size();i++)
    {
        if(!isdigit(port[i]))
        {
            std::cout << "[Error]: wrong port" << std::endl;
            return(1);
        }
    }
        
    if(password.size() < 1)
    {
        std::cout << "[Error]: password wrong size" << std::endl;
        return(1);
    }
    std::cout << av[1] << std::endl;
    Server server;
    server.initServer(av);
    return (0);
}