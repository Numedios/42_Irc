#include "../include/Serveur.hpp"

std::string Channel::sendClientName(const Client& users)
{
    std::map<int, Client *> ::iterator it = _operators.begin();

    for (; it != _operators.end(); it++)
    {
        if (it->second->getNick() == users.getNick())
            return ("@" + users.getNick());
    }
    return (users.getNick());
}


std::string Channel::sendAllClientsNames()
{
    std::string names = "";

    std::map<int, Client *> ::iterator it = _operators.begin();

    for (; it != _operators.end(); it++)
    {
      names += sendClientName(*it->second);
      names += " ";
    }

    it = _clients.begin();
    for (; it != _clients.end(); it++)
    {
        if (it->second != NULL) {
            names +=  sendClientName(*it->second);
            names += " ";
        }
    }
    return (names);
}

int Channel::checkIfClientInChannel(Client* client)
{
    std::map<int, Client *> ::iterator it = _operators.begin();

    for (; it != _operators.end(); it++)
    {
        if (it->second->getNick() == client->getNick())
            return (0);
    }
    it = _clients.begin();
    for (; it != _clients.end(); it++)
    {
        if (it->second == client)
            return (0);
    }
    return (1);
}

int Channel::checkIfClientInChannel(std::string client)
{
    std::map<int, Client *> ::iterator it = _operators.begin();

    for (; it != _operators.end(); it++)
    {
        if (it->second->getNick() == client)
            return (0);
    }
    it = _clients.begin();
    for (; it != _clients.end(); it++)
    {
        if (it->second->getNick() == client)
            return (0);
    }
    return (1);
}

void Channel::kickClient(std::string client)
{

    std::map<int, Client *>::iterator it = _operators.begin();

    for (; it != _operators.end();)
    {
        if (it->second->getNick() == client)
        {
            std::cout << "le client  " << it->second->getNick() << " a ete set a erase" << std::endl;
            std::map<int, Client *>::iterator toErase = it;
            ++it;
            _operators.erase(toErase);
        }
        else
        {
            ++it;
        }
    }
    it = _clients.begin();
    for (; it != _clients.end();)
    {
        if (it->second->getNick() == client)
        {
            std::cout << "le client  " << it->second->getNick() << " a ete set a erase" << std::endl;
            std::map<int, Client *>::iterator toErase = it;
            ++it;
            _clients.erase(toErase);
        }
        else
        {
            ++it;
        }
    }
}

void    Channel::fillModesMap() {
    _modes["+i"] = &Channel::setInvitation;
    _modes["-i"] = &Channel::removeInvitation;
    _modes["+k"] = &Channel::setKeyPass;
    _modes["-k"] = &Channel::removeKeyPass;
    _modes["+o"] = &Channel::addOperator;
    _modes["-o"] = &Channel::removeOperator;
    _modes["+l"] = &Channel::setLimitUser;
    _modes["-l"] = &Channel::removeLimitUser;
}

void    Channel::findMode(Client *client, const std::string &arg, const std::string &line) {
    std::map<std::string, ModeFunction>::iterator it = _modes.find(arg);
    if (it != _modes.end()) {
        ModeFunction func = it->second;
        (this->*func)(client, line);
    } else {
        std::cout << "Undefined mode" << std::endl;
    }
}

int     Channel::isOperator(Client *client) {
std::map<int, Client*>::const_iterator it;
    for (it = this->_operators.begin(); it != this->_operators.end(); ++it) {
        if (it->second == client) {
            return (1);
        }
    }
    return (0);
}
 
void    Channel::setInvitation(Client *client, std::string const& line) {
    this->_inviteOnly = true;
    std::cerr << "Serveur is now on invitation mode" << std::endl;
    (void) line;
}

void    Channel::removeInvitation(Client *client, std::string const& line ) {
    this->_inviteOnly = false;
    (void) line;
}

void    Channel::setKeyPass(Client *client, std::string const& pass) {
    this->_key = pass;
    std::cout << "Password has been updated" << std::endl;
}

void    Channel::removeKeyPass(Client *client, std::string const& pass) {
    this->_key.clear();
    std::cout << "Password has ben removed" << std::endl;
}

void    Channel::addOperator(Client *client, std::string const& line) {
    if (!isOperator(client)) {
        this->_operators[client->getId()] = client;
        std::cout << client->getNick() << " is now an operator" << std::endl;
    } else
        std::cout << client->getNick() << " is already an operator" << std::endl;
    (void) line;    
}

void    Channel::removeOperator(Client *client, std::string const& line) {
    if (isOperator(client)) {
        this->_operators.erase(client->getId());
        std::cout << client->getNick() << " is no more an operator" << std::endl;
    } else {
        std::cout << client->getNick() << " is not an operator" << std::endl;  
    }
}

void    Channel::setLimitUser(Client *client, std::string const& limit) {
    _maxUsers = std::atoi(limit.c_str());
}

void    Channel::removeLimitUser(Client *client, std::string const& line) {  
    _maxUsers = MAX_CLIENTS;
}