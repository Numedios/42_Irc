#include "../include/Serveur.hpp"


int handleNotice(const std::string& line, Client& client, Serveur& serveur)
{
    std::string response;
    std::vector<std::string> args = createArg(line);
    std::string message = args[args.size() - 1];
    std::string	target = args[1];

	if (message.empty())
		return 1;

	if (target.empty())
		return 1;

	if (serveur.getClient(target) == NULL && serveur.getChannel(target) == NULL)
		return 1;

    if (target[0] == '#')
	{
		Channel *channel = serveur.getChannel(target);
		
		std::map<int, Client *> clients = channel->getClients();
		std::map<int, Client *> operators = channel->getOperators();
		std::map<int, Client *>::iterator it;
		for (it = clients.begin(); it != clients.end(); it++)
		{
			if ((*it).second->getNick() != client.getNick())
            {
				response = it->second->returnPrefixe() + "NOTICE " + target  + "\r\n";
                sendResponse(*it->second, serveur, response);
            }
		}
		for (it = operators.begin(); it != operators.end(); it++)
		{
			if ((*it).second->getNick() != client.getNick())
            {
				response = it->second->returnPrefixe() + "NOTICE " + target  + "\r\n";
                sendResponse(*it->second, serveur, response);
            }
		}
	}
    else
	{
		Client *targetClient = serveur.getClient(target);
		if (targetClient == NULL)
			return 1;
	    response = client.returnPrefixe()  + "NOTICE " + target + " " + message + "\r\n";
        sendResponse(*serveur.getClient(target), serveur, response);
	}
    return (0);
}

/*

10:35:55:-->   {4}[:sofiane!sbelabba@127.0.0.1 NOTICE sofiane :lol

*/