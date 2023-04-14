#include "Server.hpp"

int main(int ac, char **av)
{
    if(ac != 3)
    {
        std::cout << "./irc [port] [password]" << std::endl;
        return(1);
    }
    std::string password(av[2]);
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