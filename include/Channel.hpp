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
    typedef void (Channel::*ModeFunction)(Client*, const std::string&);

    void fillModesMap();
    void findMode(Client *client, std::string const& arg, std::string const& line);
    int  isOperator(Client *client);
    void setInvitation(Client *client, std::string const& line);
    void removeInvitation(Client *client, std::string const& line );
    void setKeyPass(Client *client, std::string const& pass);
    void removeKeyPass(Client *client, std::string const& pass);
    void addOperator(Client *client, std::string const& line);
    void removeOperator(Client *client, std::string const& line);
    void setLimitUser(Client *client, std::string const& limit);
    void removeLimitUser(Client *client, std::string const& line);
    void removeTopicProtection(Client *client, const std::string& line);
    void setTopicProtection(Client *client, const std::string& line);

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

    bool getInviteStatus() { 
        return _inviteOnly; 
    }

    void setMaxUsers(int max) {
        _maxUsers = max;
    }

    int getMaxUsers() { return _maxUsers; }

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
