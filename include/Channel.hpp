#ifndef DEF_CHANNEL
#define DEF_CHANNEL

#include <iostream>
#include <string>
#include <vector>
#include <map>

class Client;

class Channel {
public:
    Channel() : _name("") {}
    Channel(const std::string& name) : _name(name), _topic(""), _key("") ,_inviteOnly(false) {}
    std::string sendClientName(const Client& users);
    std::string sendAllClientsNames();
    int checkIfClientInChannel(Client* client);
    int checkIfClientInChannel(std::string client);
    void kickClient(std::string client);
    void removeOperator(Client *A, Client *B, bool o);
    void setInvitation(Client *client, bool i);
    void setKeyPass(Client *client, bool k, std::string pass);
    void setLimitUser(Client *client, bool l, int limit);
    int  isOperator(Client * client);

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

    void setTopic(std::string name)
    {
        _topic = name;
    }

    void setInviteStatus(bool status) { this->_inviteOnly = status; }

    std::string& getChannelName(){
        return _name;
    }
    
    std::string& getTopic(){
        return _topic;
    }

    std::map<int, Client *> getClients(){
        return _clients;
    }

    std::map<int, Client *>	getOperators(){
        return _operators;
    }

    void setOperator(Client* client){
        _operators[client->getSocket()] = client;
    }

    void addClient(Client *client){
        _clients[client->getSocket()] = client;
    }

    bool getInviteStatus() { return this->_inviteOnly; }

    void setMaxUsers(int max) {
        _maxUsers = max;
    }

    int getMaxUsers() { return _maxUsers; }

private:

    std::string				_name;
	std::string				_topic;
	std::string				_key;
	std::map<int, Client *>	_operators;
	bool					_inviteOnly;
	std::map<int, Client *>	_clients;
	std::map<int, Client *>	_invitedList;
    int                     _maxUsers;


};

#endif
