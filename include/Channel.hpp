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

    Client*	getOperator(){
        return _operator;
    }

    void setOperator(Client* client){
        _operator = client;
    }

    void addClient(Client *client){
        _clients[client->getSocket()] = client;
    }

    bool getInviteStatus() { return this->_inviteOnly; }

private:

    std::string				_name;
	std::string				_topic;
	std::string				_key;
	Client*					_operator;
	bool					_inviteOnly;
	std::map<int, Client *>	_clients;
	std::map<int, Client *>	_invitedList;

};

#endif
