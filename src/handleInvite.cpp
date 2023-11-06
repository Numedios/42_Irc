#include "../include/Serveur.hpp"

int handleInvite(const std::string& line, Client& client, Serveur& serveur)
{
    std::vector<std::string> args = createArg(line);
    if (args.size() < 3) 
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }
    std::string nicknameStr = args[1];
    if (serveur.getClient(nicknameStr) == NULL)
    {
        std::string response = client.returnPrefixe() + ERR_NOSUCHNICK(nicknameStr) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    Client * targetClient = serveur.getClient(nicknameStr);
    std::string channelStr = args[2];
    Channel* channel = serveur.getChannel(channelStr);
    if (channel)
    {
        if (channel->checkIfClientInChannel(client.getNick()) == 1)
        {
            std::string response = client.returnPrefixe() + ERR_NOSUCHNICK(nicknameStr) + "\r\n";
            sendResponse(client, serveur, response);
            return 1;
        }
        if (!(channel->checkIfClientInChannel(serveur.getClient(nicknameStr))))
        {
            std::cout <<  "nickbane " << nicknameStr << std::endl;
            std::string response = client.returnPrefixe() + ERR_USERONCHANNEL(targetClient->getNick(), channelStr) + "\r\n";
            sendResponse(client, serveur, response);
            return 1;
        }
    }
    if (channel->getInviteStatus() == true && channel->checkIfClientOperator(&client) != 0)
	{
        std::string response = client.returnPrefixe() + ERR_CHANOPRIVSNEEDED(channelStr) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }
    channel->addInvitedClient(targetClient);

    std::string response = targetClient->returnPrefixe() + "INVITE " + nicknameStr + " " + channelStr + "\r\n";
    sendResponse(*targetClient, serveur, response);
    response = client.returnPrefixe() + RPL_INVITING(channelStr, nicknameStr) + "\r\n";
    sendResponse(client, serveur, response);
    return (0);
}
