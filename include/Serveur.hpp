#ifndef SERVEUR_HPP
#define SERVEUR_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unistd.h>
#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <csignal>
#include <queue>
#include <typeinfo>
#include <map>
#include <sstream>
#include <fcntl.h>
#include "Client.hpp"
#include "Channel.hpp"
#include "Responses.hpp"

extern bool	g_kill;

class Serveur;
class Client;
class Channel;

typedef int (*FunctionPtr)(const std::string&, Client&, Serveur&);
typedef int (*ModeFunction)(std::vector<std::string>&, Client&, Serveur&, Channel&);
void fillModesMap(std::map<std::string, ModeFunction>& modes);
void fillCommandMapAuth(std::map<std::string, FunctionPtr>& commands);
void fillCommandMap(std::map<std::string, FunctionPtr>& commands);
std::string getNthWord(const std::string& input, int place);
void mergeQueues(std::queue<std::string>& q1, std::queue<std::string>& q2);
std::queue<std::string> splitToQueue(const std::string& input, char delimiter);
std::string convertIntToString(int value) ;
std::string create_message(Client client, std::string capted, std::string message);
void sendResponse(Client& client, Serveur& server, const std::string& response);
std::string convertIntToString(int value);
std::vector<std::string> createArg(const std::string& line);
void printVector(std::vector<std::string>& vec);
int handleJoin(const std::string& line, Client& client, Serveur& serveur);
std::string createReason(const std::string& line, int place);
std::string createReasonMode(const std::string& line, int place);


const int MAX_CLIENTS = 10;

class Serveur {
public:
    Serveur(int port, char *password);
    Serveur();
    void run();
    void printInfoServeur();
    int acceptClient();
    int processReceivedData(int i);
    void handleClientDisconnect(int index, std::string reason);
    int processAuthenticationManagement(int index);
    int processCommandManagement(int i);
    void displayHistory();
    int checkChannelInServeur(std::string name);
    void deleteChannel(std::string name);
    void kickClientFromChannel(Client* client, Channel* channel);
    void closeServer();

    ~Serveur() 
    {
        std::map<int, Client *>::iterator client = _clients.begin();

        for (; client != _clients.end(); ++client) {
            delete client->second;
            
        }
        for (int i = 0; i < _numClients; ++i) {
        if (fds[i].fd != -1) {
            if (close(fds[i].fd) == -1) {
                // Vous pouvez gérer les erreurs de fermeture de fichier en fonction de vos besoins
            }
            fds[i].fd = -1; // Réinitialiser le descripteur de fichier après la fermeture
        }
    }
    }

    void	kickClientFromAllChannelsWithJoin(Client* client, std::string reason)
    {
        std::vector<Channel *>	channelsWhereClientIs;

        for (std::map<std::string, Channel>::iterator it = this->_channels.begin(); it != this->_channels.end(); it++)
        {
            if ((*it).second.checkIfClientInChannel(client->getNick()) == 0)
            {
        
                channelsWhereClientIs.push_back(&(*it).second);
            }
        }
        while (channelsWhereClientIs.size() > 0)
        {
            std::string response = client->returnPrefixe() + "PART " + channelsWhereClientIs[0]->getChannelName() + " " + reason + "\r\n";
            channelsWhereClientIs[0]->sendMessageToAllExceptOne(response, *this, *client);
            channelsWhereClientIs[0]->delUser(client);
            if (channelsWhereClientIs[0]->getNumberClient() == 0)
                deleteChannel(channelsWhereClientIs[0]->getChannelName());
            channelsWhereClientIs.erase(channelsWhereClientIs.begin());
        }
    }

    void allClientIndexDown(int index)
    {
        std::map<int , Client *>::iterator it =  _clients.begin();

        for (; it != _clients.end(); it++)
        {
            if (it->second->getId() >= index)
                (it)->second->setId(it->second->getId() - 1);
        }
    }
    
    int getServeurSocket(){
        return _serverSocket;
    }

    int getNumClients(){
        return _numClients;
    }

    int getFdsSocket(int numClient){
        return fds[numClient].fd;
    }

    std::string getPassword()
    {
        return _password;
    }

    std::map<int , Client *>&  getClients(){
        return _clients;
    }

    Client* getClient(int fd){
        return _clients[fd];
    }

    Client* getClient(std::string name){
        std::map<int , Client *>::iterator it = _clients.begin();
        for(; it != _clients.end(); it++)
        {
            if (it->second->getNick() == name)
                return (it->second);
        }
        return (NULL);
    }



    std::map<std::string, FunctionPtr>&  getCommands()
    {
        return _commands;
    }

    std::map<std::string, FunctionPtr>& getAuth(){
        return _auth;
    }

    Channel* getChannel(std::string name)
    {
        std::map<std::string, Channel>::iterator	it;

        it = this->_channels.find(name);
        if (it == this->_channels.end())
            return NULL;
        return &(*it).second;
    }

    std::map<std::string, Channel>& getChannels()
    {
        return _channels;
    }

    void addHistoryChat(std::string message) {
        _historyChat.push_back(message);
    }

    std::queue<std::string>& getmessageQueue(){
        return _messageQueue;
    }

    void setNumCLients(int numClients)
    {
        _numClients = numClients;
    }

    void setFds(int clientSocket, int numClients)
    {
        fds[numClients].fd = clientSocket;
        fds[numClients].events = POLLIN;
        fds[numClients].revents = 0;
    }
    
    void addChannel(std::string name,Channel& channel) {
        _channels[name] = channel;
    }

    void delChannel(const std::string& name) {
        _channels.erase(name);
    }

private:

    int _port;  //rajouter parsing des port
    int _serverSocket; 
    int _numClients; //x
    std::string _password;
    struct pollfd fds[MAX_CLIENTS + 1];

    std::map<int , Client *> _clients;
    std::map<std::string, FunctionPtr>  _commands;
    std::map<std::string, FunctionPtr>  _auth; 
    std::map<std::string, Channel>		_channels;
    std::vector<std::string> _historyChat; // historique des messages emis et recu 
    std::queue<std::string> _messageQueue; // file ordre de traitement des messages
};

#endif
