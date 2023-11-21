#include "../include/Serveur.hpp"


/*

args[0] = PRIVMSG 
args[1] = #channel 
args[2 et +] = message a envoyer

*/

int handlePrivmsg(const std::string& line, Client& client, Serveur& serveur)
{
	std::vector<std::string> args = createArg(line);
	std::string response;

    if (args.size() < 2) 
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 


    std::string message = args[args.size() - 1];
	std::string reason   = createReason(line, 1);

	std::string nameChannel = args[1];
	


	if (serveur.getClient(nameChannel) != NULL)
	{
		response = client.returnPrefixe() + "PRIVMSG " + nameChannel  + " :" + reason + "\r\n";
		sendResponse(*serveur.getClient(nameChannel), serveur, response);
		return (1);
	}
	Channel* channel = serveur.getChannel(nameChannel);


	if (args[1][0] != '#' || channel == NULL) // 2
    {
        std::string response = client.returnPrefixe() + ERR_NOSUCHCHANNEL(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

	if (channel->checkIfClientInChannel(client.getNick()) == 1) // 3
    {
        std::string response = client.returnPrefixe() + ERR_NOTONCHANNEL(nameChannel)+ "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

	if (message.empty())
	{
		std::string response = client.returnPrefixe() + ERR_NOTEXTTOSEND() + "\r\n";
    
    	sendResponse(client, serveur, response);
		return 1;
	}

	if (nameChannel.empty())
	{
		std::string response = client.returnPrefixe() + ERR_NORECIPIENT(args[0]) + "\r\n";
    
    	sendResponse(client, serveur, response);
		return 1;
	}

    if (nameChannel[0] == '#')
	{

		response = client.returnPrefixe() + "PRIVMSG " + nameChannel  + " " + reason + "\r\n";
		channel->sendMessageToAllExceptOne(response, serveur, client);
	}
    return (0);
}