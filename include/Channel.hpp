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
    Channel(const std::string& name) : _name(name) {}

    std::string& getChannelName(){
        return _name;
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

    std::string sendClientName(const Client& users)
    {
        if (_operator->getNick() == users.getNick())
            return ("@" + users.getNick());
        return (users.getNick());
    }


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
