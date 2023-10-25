#include "../include/Serveur.hpp"

std::string Channel::sendClientName(const Client& users)
{
    if (_operator != NULL && _operator->getNick() == users.getNick())
        return ("@" + users.getNick());
    return (users.getNick());
}


std::string Channel::sendAllClientsNames()
{
    std::string names = "";

    if (_operator != NULL) //verifier que un operateur existe
        names += sendClientName(*_operator);
    names += " ";
    std::map<int, Client *>::iterator it = _clients.begin();
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
    if (_operator != NULL && client == _operator)
        return 0;
    std::map<int, Client *>::iterator it = _clients.begin();
    for (; it != _clients.end(); it++)
    {
        if (it->second == client)
            return (0);
    }
    return (1);
}

int Channel::checkIfClientInChannel(std::string client)
{
    if (_operator != NULL && client == _operator->getNick())
        return 0;
    std::map<int, Client *>::iterator it = _clients.begin();
    for (; it != _clients.end(); it++)
    {
        if (it->second->getNick() == client)
            return (0);
    }
    return (1);
}

void Channel::kickClient(std::string client)
{
    if (_operator != NULL && client == _operator->getNick())
        _operator = NULL;
    std::map<int, Client *>::iterator it = _clients.begin();
    for (; it != _clients.end();)
    {
        if (it->second->getNick() == client)
        {
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
