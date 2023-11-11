#include "../include/Serveur.hpp"


int handlePrivmsg(const std::string& line, Client& client, Serveur& serveur)
{
	std::vector<std::string> args = createArg(line);

    if (args.size() < 2) 
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 

    std::string response;
    std::string message = args[args.size() - 1];
    std::string	target = args[1];
	std::string reason   = createReason(line, 1);


	std::cout << "PRIVMSG" << std::endl;
	if (message.empty())
	{
		std::string response = client.returnPrefixe() + ERR_NOTEXTTOSEND() + "\r\n";
    
    	sendResponse(client, serveur, response);
		return 1;
	}
	if (target.empty())
	{
		std::string response = client.returnPrefixe() + ERR_NORECIPIENT(args[0]) + "\r\n";
    
    	sendResponse(client, serveur, response);
		return 1;
	}
	if (serveur.getClient(target) == NULL && serveur.getChannel(target) == NULL)
	{
		std::string response = client.returnPrefixe() + ERR_NOSUCHNICK(target) + "\r\n";
    
    	sendResponse(client, serveur, response);
		return 1;
	}
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
				response = client.returnPrefixe() + "PRIVMSG " + target  + " " + reason + "\r\n";
                sendResponse(*it->second, serveur, response);
            }
		}
		for (it = operators.begin(); it != operators.end(); it++)
		{
			if ((*it).second->getNick() != client.getNick())
            {
				response = client.returnPrefixe() + "PRIVMSG " + target  + " " + reason + "\r\n";
                sendResponse(*it->second, serveur, response);
            }
		}
	}
    else
	{
		Client *targetClient = serveur.getClient(target);
		if (targetClient == NULL)
		{
			std::string response = client.returnPrefixe() + ERR_NOSUCHNICK(target) + "\r\n";
    		sendResponse(client, serveur, response);
			return 1;
		}
	    response = client.returnPrefixe()  + "PRIVMSG " + target + " " + reason + "\r\n";
        sendResponse(*serveur.getClient(target), serveur, response);
	}
    return (0);
}


/*

16:41:40:   <--{4}[PRIVMSG #lol :b
16:41:40:-->   {5}[:sofiane!sbelabba@127.0.0.1 PRIVMSG #lol :b
16:41:42:   <--{5}[PRIVMSG #lol :b
16:41:42:-->   {4}[:said!sbelabba@127.0.0.1 PRIVMSG #lol :b

16:43:43:   <--{4}[PRIVMSG #lol :b
16:43:43:-->   {5}[:sofiane!sbelabba@127.0.0.1 PRIVMSG #lol :b
16:43:45:   <--{5}[PRIVMSG #lol :b
16:43:45:-->   {4}[:said!sbelabba@127.0.0.1 PRIVMSG #lol :b



*/