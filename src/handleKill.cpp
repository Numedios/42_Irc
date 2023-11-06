#include "../include/Serveur.hpp"


int handleKill(const std::string& line, Client& client, Serveur& serveur)
{
	std::string	buffer;
    std::vector<std::string> args = createArg(line);

	// if (!client->isModeInUse('o'))
	// {
    // 	std::string response = client.returnPrefixe() + ERR_NOPRIVILEGES()+ "\r\n";
    //     sendResponse(client, serveur, response);
	// 	return 1;
    // }
	if (args.size() < 3 || args[2] == ":")
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
		return 1;
    }
	Client*	victim = serveur.getClient(args[1]);
	if (!victim)
    {
        std::string response = client.returnPrefixe() + ERR_NOSUCHNICK(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
		return 1;
    }
	size_t	posStartComment = line.find(':') + 1;

	buffer = line.substr(posStartComment, line.size() - posStartComment - 1);
	std::string response = victim->returnPrefixe() + "KILL :" + buffer + "\r\n";
    sendResponse(client, serveur, response);
	serveur.handleClientDisconnect(victim->getId(), "<KILLED>");
	return (0);
}
