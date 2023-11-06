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
