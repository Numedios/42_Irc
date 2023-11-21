#ifndef DEF_CHANNEL
#define DEF_CHANNEL

#include <iostream>
#include <string>
#include <vector>
#include <map>



class Client;
class Serveur;
class Channel;

typedef int (*ModeFunction)(std::vector<std::string>&, Client&, Serveur&, Channel&);
void fillModesMap(std::map<std::string, ModeFunction>& modes);





class Channel {
public:
    Channel();
    Channel(const std::string& name);
    std::string sendClientName(const Client& users);
    std::string sendAllClientsNames();
    int checkIfClientInChannel(Client* client);
    int checkIfClientInChannel(std::string client);
    void kickClient(std::string client);
    int  isOperator(Client *client);
    void sendMessageToAll(std::string response,  Serveur& serveur);
    void sendMessageToAllExceptOne(std::string response,  Serveur& serveur,Client& user);

    Client * getClient(std::string client)
    {
        std::map<int, Client *>::iterator it = _clients.begin();

        for (; it != _clients.end(); it++)
        {
            if (it->second->getNick() == client)
                return (it->second);
        }
        return (it->second);
    }

    Client * getOperator(std::string client)
    {
        std::map<int, Client *>::iterator it = _operators.begin();

        for (; it != _operators.end(); it++)
        {
            if (it->second->getNick() == client)
                return (it->second);
        }
        return (it->second);
    }

    Client * getOperator(Client* client)
    {
        std::map<int, Client *>::iterator it = _operators.begin();

        for (; it != _operators.end(); it++)
        {
            if (it->second->getNick() == client->getNick())
                return (it->second);
        }
        return (it->second);
    }

    int checkIfClientOperator(std::string client)
    {
        std::map<int, Client *>::iterator it = _operators.begin();

        for (; it != _operators.end(); it++)
        {
            if (it->second->getNick() == client)
                return (0);
        }
        return (1);
    }

    int checkIfClientOperator(Client* client)
    {
        std::map<int, Client *>::iterator it = _operators.begin();

        for (; it != _operators.end(); it++)
        {
            if (it->second->getNick() == client->getNick())
                return (0);
        }
        return (1);
    }

    int checkIfClientInvited(std::string client)
    {
        std::map<int, Client *>::iterator it = _invitedList.begin();

        for (; it != _invitedList.end(); it++)
        {
            if (it->second->getNick() == client)
                return (0);
        }
        return (1);
    }

    int checkIfClientInvited(Client* client)
    {
        std::map<int, Client *>::iterator it = _invitedList.begin();

        for (; it != _invitedList.end(); it++)
        {
            if (it->second->getNick() == client->getNick())
                return (0);
        }
        return (1);
    }

    bool	isClientInvited(Client* client) {
        if (this->_invitedList.find(client->getSocket()) == this->_invitedList.end())
            return false;

        return true;
    }

    void	addInvitedClient(Client* client) {
        if (isClientInvited(client))
            return ;

        this->_invitedList[client->getSocket()] = client;
    }

    void	delInvitedClient(Client* client) {
        _invitedList.erase(client->getSocket());
    }


    void	delOperator(Client* client) {
        _operators.erase(client->getSocket());
    }

    void	delClient(Client* client) {
        _clients.erase(client->getSocket());
    }

    void	delUser(Client* client) {
        if (checkIfClientOperator(client->getNick()) == 0 && _operators.size() == 1)
        {
            if (!_clients.empty())
            {
                std::map<int, Client *>::iterator it = _clients.begin();
                Client * tmp = it->second;
                _operators[tmp->getSocket()] = tmp;
                _clients.erase(tmp->getSocket());
            }
        }
        _operators.erase(client->getSocket());
        _clients.erase(client->getSocket());
        _invitedList.erase(client->getSocket());

    }

    void setTopic(std::string name)
    {
        _topic = name;
    }

    void setInviteStatus(bool status) { this->_inviteOnly = status; }

    void setTopicProtection(bool status) {_topicProtection = status; }

    void setKey(std::string key) 
    { 
        _key = key; 
    }

    void clearKey() {
        _key.clear();
    }
    
    void setMaxUsers(int max) {
        _maxUsers = max;
    }


    std::string& getChannelName(){
        return _name;
    }
    
    std::string& getTopic(){
        return _topic;
    }

    std::string& getKey(){
        return _key;
    }

    std::map<int, Client *>& getClients(){
        return _clients;
    }

    std::map<int, Client *>	getOperators(){
        return _operators;
    }

    int  getNumberClient(){
        return (_operators.size() + _clients.size());
    }

    std::map<std::string, ModeFunction>& getMods() {
        return _modes;
    }

    void setOperator(Client* client){
        _operators[client->getSocket()] = client;
    }

    void addClient(Client *client){
        _clients[client->getSocket()] = client;
    }

    bool hasKey() const {
        return !_key.empty();
    }

    bool getInviteStatus() { 
        return _inviteOnly; 
    }

    bool getTopicStatus() { 
        return _topicProtection; 
    }

    int getMaxUsers() { 
        return _maxUsers; }

private:

    std::string				_name;
	std::string				_topic;
	std::string				_key;
	std::map<int, Client *>	_operators;
    bool                    _topicProtection;
	bool					_inviteOnly;
	std::map<int, Client *>	_clients;
	std::map<int, Client *>	_invitedList;
    int                     _maxUsers;
    std::map<std::string, ModeFunction> _modes;
};

#endif
