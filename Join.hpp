#ifndef JOIN_HPP
# define JOIN_HPP
# include <iostream>
# include <vector>
# include <memory>
# include "ACommand.hpp"

class ACommand;

class Join : public ACommand
{
    public:

        Join();
        Join(Join const & src);
        Join & operator=(Join const & rhs);
        ~Join();

        void execCommand();

    private:

};

#endif