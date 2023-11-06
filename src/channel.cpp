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

int     Channel::isOperator(Client *client) {
std::map<int, Client*>::const_iterator it;
    for (it = this->_operators.begin(); it != this->_operators.end(); ++it) {
        if (it->second == client) {
            return (1);
        }
    }
    return (0);
}

void    Channel::setInvitation(Client *client, bool i) {
    if (!isOperator(client)) {
        std::cout << "You don't have the correct access to do this\n" << std::endl;
        return ;
    }
    this->_inviteOnly = i;   
}

void    Channel::setKeyPass(Client *client, bool k, std::string pass) {
    if (!isOperator(client)) {
        std::cout << "You don't have the correct access to do this" << std::endl;
        return ;
    }
    if (k) {
        this->_key.clear();
        std::cout << "Password has ben removed" << std::endl;
    } else {
        this->_key = pass;
        std::cout << "Password has been updated" << std::endl;
    }
}

void    Channel::removeOperator(Client *A, Client *B, bool o) {
    if (!isOperator(A)) {
        std::cout << "You don't have the correct access to do this" << std::endl;
        return ;
    }
    if (o) {
        if (!isOperator(B)) {
            this->_operators[B->getId()] = B;
            std::cout << B->getNick() << " is now an operator" << std::endl;
        } else
            std::cout << B->getNick() << " is already an operator" << std::endl;
    } else {
        if (isOperator(B)) {
            this->_operators.erase(B->getId());
            std::cout << B->getNick() << " is no more an operator" << std::endl;
        } else
            std::cout << B->getNick() << " is not an operator" << std::endl;  
    }
}

void    Channel::setLimitUser(Client *client, bool l, int limit) {
    if (!isOperator(client)) {
        std::cout << "You don't have the correct access to do this" << std::endl;
        return ;
    }
    if (l)
        _maxUsers = limit;
    else
        limit = MAX_CLIENTS;
}
