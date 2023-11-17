#include "../include/Serveur.hpp"

int handlePart(const std::string& line, Client& client, Serveur& serveur)
{
    std::vector<std::string> args = createArg(line);
	std::string response;

    if (args.size() < 3) 
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

	std::cout << "la1******************" << std::endl;
	if (serveur.getChannel(args[1]) == NULL) 
	{
		std::cout << "la2******************" << std::endl;
		if (channel.checkIfClientInChannel(client.getNick()))
		{
			response = client.returnPrefixe() + ERR_NOTONCHANNEL(args[1])+ "\r\n";
        	sendResponse(client, serveur, response);
        	return 1;
		}
		std::cout << "la3******************" << std::endl;
		response = client.returnPrefixe() + " PART " + args[1] + " :see you guys!" + "\r\n";
		sendResponse(client, serveur, response);
		channel.kickClient(client.getNick());
	}

    return 0;
}
