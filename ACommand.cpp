#include "ACommand.hpp"
#include "Join.hpp"

ACommand::ACommand()
{

}

ACommand::ACommand(std::string name) : _name(name), _size(name.size())
{

}

ACommand::ACommand(ACommand const & src)
{
    *this = src;
}

ACommand &ACommand::operator=(ACommand const & rhs)
{
    (void)rhs;
    return (*this);
}

ACommand::~ACommand()
{

}

void	ACommand::launchCommand(std::string buffer)
{
	std::vector<ACommand *> commands;

    commands.push_back((new Join()));
	for (std::vector<ACommand *>::iterator it = commands.begin(); it != commands.end(); it++)
    {
        if ((*it)->_name.compare(buffer) == 0)
            (*it)->execCommand();
    }
}

/*int ACommand::check_command(std::string buffer){
    if (buffer.size() < _size || buffer.compare(0, _size, _name))
        return (0);
    return (1);
}*/