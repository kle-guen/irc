#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP
# include <iostream>
# include <vector>
# include <memory>

class Join;

class ACommand
{
    public:

        ACommand();
        ACommand(std::string name);
        ACommand(ACommand const & src);
        ACommand & operator=(ACommand const & rhs);
        ~ACommand();

        static void launchCommand(std::string buffer);

        virtual void execCommand() = 0;
        

    private:


        const std::string _name;
        unsigned int _size; 

};

#endif