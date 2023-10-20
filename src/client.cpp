#include "../include/Client.hpp"



void Client::printInfoClient() 
{
    std::cout << "_id             = " << _id << std::endl;
    std::cout << "_socket         = " << _socket << std::endl;
    std::cout << "_isConnected    = " << (_isConnected ? "true" : "false") << std::endl;
    std::cout << "_authenticated  = " << (_authenticated ? "true" : "false") << std::endl;
    std::cout << "_pendingCommand = " << _pendingCommand << std::endl;
    std::cout << "_currentChannel = " << _currentChannel << std::endl;
    std::cout << "_nick           = " << _nick << std::endl;
    std::cout << "_userName       = " << _userName << std::endl;
    std::cout << "_ip             = " << _ip << std::endl;
    std::cout << "_authState      = " << _authState << std::endl;
}

std::string Client::returnPrefixe()
{
    return (":" + _nick + "!" + _userName + "@127.0.0.1" + " ");
}