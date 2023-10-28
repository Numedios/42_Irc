#include "../include/Serveur.hpp"


int handleTopic(const std::string& line, Client& client, Serveur& serveur)
{
    std::vector<std::string> args = createArg(line);
    if (args.size() < 2) 
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }
    Channel *channel = serveur.getChannel(args[1]);
    if (!channel || !channel->checkIfClientInChannel(client.getNick()))
    {
        std::string response = client.returnPrefixe() + ERR_NOTONCHANNEL(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    std::string newTopic = args[args.size() - 1];
    std::map<int, Client *> clients = channel->getClients();
	std::map<int, Client *>::iterator it;

    return (0);
}
