#include "Server.hpp"

int main(int ac, char **av)
{
    if(ac != 3)
    {
        std::cout << "./irc [port] [password]" << std::endl;
        return(1);
    }
    Server server;
    server.initServer(av);
    return (0);
}

