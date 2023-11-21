#include "../include/Serveur.hpp"

int handleMode(const std::string& line, Client& client, Serveur& serveur);
int handleKick(const std::string& line, Client& client, Serveur& serveur);
int handleQuit(const std::string& line, Client& client, Serveur& serveur);
int handleInvite(const std::string& line, Client& client, Serveur& serveur);
int handleTopic(const std::string& line, Client& client, Serveur& serveur);
int handleNotice(const std::string& line, Client& client, Serveur& serveur);
int handlePrivmsg(const std::string& line, Client& client, Serveur& serveur);
int handleKill(const std::string& line, Client& client, Serveur& serveur);
int handleNick(const std::string& line, Client& client, Serveur& serveur);
int handlePart(const std::string& line, Client& client, Serveur& serveur);

int handlePing(const std::string& line, Client& client, Serveur& serveur) 
{
    (void) line;
    std::string response = client.returnPrefixe() + PONG(client.getNick()) + "\r\n";
   
    sendResponse(client, serveur, response);
    return (0);
}



void fillCommandMap(std::map<std::string, FunctionPtr>& commands)
{
    commands["JOIN"] = handleJoin;
    commands["PING"] = handlePing;
    commands["MODE"] = handleMode;
    commands["KICK"] = handleKick;
    commands["QUIT"] = handleQuit;
    commands["INVITE"] = handleInvite;
    commands["TOPIC"] = handleTopic;
    commands["NOTICE"] = handleNotice;
    commands["PRIVMSG"] = handlePrivmsg;
    //commands["kill"] = handleKill; // inutile
    commands["NICK"] = handleNick;
    //commands["PART"] = handlePart;
}
