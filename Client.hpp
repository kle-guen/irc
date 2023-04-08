#ifndef Client_HPP
#define Client_HPP

#include <iostream>
#include <string>

class Client
{
private:
    std::string nick_name;
    std::string user_name;
    std::string command;
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

    std::string getNick_name(){
        return(nick_name);
    }

    std::string getUser_name(){
        return(user_name);
    }

    void setStatus(int n){
        status = n;
    }

    void appendCommand(std::string str){
        command.append(str);
    }

    void eraseBackslash_N(){
        int position = command.find_first_of('\n');
        if(position == -1)
        {
            std::cout << "no backslash_N found" << std::endl;
            return;
        }
        command.erase(position,position + 1);
    }

    std::string getCommand(){
        return(command);
    }
    
    void resetCommand(){
        command.clear();
    }
    
    void setNick_name(std::string name){
        nick_name = name;
    }

    void setUser_name(std::string name){
        user_name = name;
    }
    //functions

};
std::ostream & operator<<(std::ostream & o, Client const & rhs);

#endif