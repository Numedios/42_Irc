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
    std::string sendClientName(const Client& users);
    std::string sendAllClientsNames();
    int checkIfClientInChannel(Client* client);
    int checkIfClientInChannel(std::string client);
    void kickClient(std::string client);

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
