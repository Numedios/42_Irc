#include "../include/Serveur.hpp"

int handlePart(const std::string& line, Client& client, Serveur& serveur)
{
    std::vector<std::string> args = createArg(line);
	std::string response;

    if (args.size() < 2) 
    {
        response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 
	if (args[1][0] != '#')
    {
        std::string response = client.returnPrefixe() + ERR_NOSUCHCHANNEL(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }
	Channel& channel = *serveur.getChannel(args[1]);

	if (serveur.getChannel(args[1]) != NULL) 
	{
		if (channel.checkIfClientInChannel(client.getNick()))
		{
			response = client.returnPrefixe() + ERR_NOTONCHANNEL(args[1])+ "\r\n";
        	sendResponse(client, serveur, response);
        	return 1;
		}
		response = client.returnPrefixe() + " PART " + args[1] + " :see you guys!" + "\r\n";
		sendResponse(client, serveur, response);
		channel.kickClient(client.getNick());
		return 0;
	}
	response = client.returnPrefixe() + ERR_NOSUCHCHANNEL(channel.getChannelName()) + "\r\n";
	sendResponse(client, serveur, response);
    return 1;
}
