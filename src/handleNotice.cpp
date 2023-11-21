#include "../include/Serveur.hpp"






/*

args[0] = Notice
args[1] = #lol ou said (channel ou user) 
args[2 et +] message a renvoyer

*/



int handleNotice(const std::string& line, Client& client, Serveur& serveur)
{
    std::string response;
    std::vector<std::string> args = createArg(line);
    

	if (args.size() < 3)  // 1
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 

	std::string	target = args[1];
	std::string reason   = createReason(line, 1);
	Channel* channel = serveur.getChannel(target);

	if (target[0] == '#')
	{
		
		if (channel == NULL) // 2
    	{
        std::string response = client.returnPrefixe() + ERR_NOSUCHCHANNEL(target) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
   		}
		response = client.returnPrefixe()  + "NOTICE " + target + " " + reason + "\r\n";
		channel->sendMessageToAll(response, serveur);
		return (1);
	}
	Client *targetClient = serveur.getClient(args[1]);

	if (targetClient == NULL)
    {
        response = client.returnPrefixe() + ERR_NOSUCHNICK(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }
	response = client.returnPrefixe()  + "NOTICE " + args[1] + " " + reason + "\r\n";
    sendResponse(*targetClient, serveur, response);
    return (0);
}
