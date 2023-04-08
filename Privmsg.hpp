#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP
# include <iostream>
# include <vector>
# include <memory>
# include "ACommand.hpp"

class ACommand;

class Privmsg : public ACommand
{
    public:

        Privmsg();
        Privmsg(Privmsg const & src);
        Privmsg & operator=(Privmsg const & rhs);
        ~Privmsg();

        void execCommand();

    private:

};

#endif