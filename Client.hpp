#ifndef Client_HPP
#define Client_HPP

#include <iostream>
#include <string>


typedef struct s_mode_user
{
    bool i;
    bool s;
    bool w;
}   t_mode_user;

class Client
{
private:
    std::string nick_name;
    std::string user_name;
    std::string command;
    t_mode_user _mode;
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
    
    int getInvisible(){
        return(_mode.i);
    }

    int getNotification(){
        return(_mode.s);
    }

    int getWallops(){
        return(_mode.w);
    }

    void setWallops(int n){
        _mode.w = n;
    }
    
    void setNotification(int n){
        _mode.s = n;
    }

    void setInvisible(int n){
        _mode.i = n;
    }

    void setStatus(int n){
        status = n;
    }

    void appendCommand(std::string str){
        command.append(str);
    }

    void eraseToBackslash_N(){
        int position = command.find_first_of('\n');
        if(position == -1)
            return;
        command.erase(0,position + 1);
    }

    void eraseBackslash_R(){
        while(command.find_first_of('\r'))
        {
            int position = command.find_first_of('\r');
            if(position == -1)
                return;
            command.erase(position,1);
        }
    }

    std::string getCommand(){
        return(command);
    }
    
    void resetCommand(){
        command.clear();
    }
    
    void setNickName(std::string name){
        nick_name = name;
    }

    void setUserName(std::string name){
        user_name = name;
    }
    //functions

};
std::ostream & operator<<(std::ostream & o, Client const & rhs);

#endif