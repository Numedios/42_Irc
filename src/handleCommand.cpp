#include "../include/Serveur.hpp"

int handleMode(const std::string& line, Client& client, Serveur& serveur);

int handlePing(const std::string& line, Client& client, Serveur& serveur) 
{
    std::string response = client.returnPrefixe() + PONG(client.getNick()) + "\r\n";
   
   sendResponse(client, serveur, response);
    return (0);
}



void fillCommandMap(std::map<std::string, FunctionPtr>& commands)
{
    commands["JOIN"] = handleJoin;
    commands["PING"] = handlePing;
    commands["MODE"] = handleMode;
    // commands["KICK"] = handleKick;
}