#ifndef CHANNEL_HPP
# define CHANNEL_HPP
# include <iostream>

class Channel
{
    public:

        Channel();
        Channel(Channel const & src);
        Channel & operator=(Channel const & rhs);
        ~Channel();

    private:

};

#endif