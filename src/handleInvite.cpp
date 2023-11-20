#include "../include/Serveur.hpp"



/*

args[0] = INVITE
args[1] = said (client inviter)
args[2] = #lol (channel name)

*/


int handleInvite(const std::string& line, Client& client, Serveur& serveur)
{
    std::vector<std::string> args = createArg(line);
    if (args.size() < 3) // nombre d'argument
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    std::string channelStr = args[2];
    Channel* channel = serveur.getChannel(channelStr);

    if (args[2][0] != '#' || channel == NULL)
    {
        std::string response = client.returnPrefixe() + ERR_NOSUCHCHANNEL(args[2]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    if (channel->checkIfClientInChannel(client.getNick()) == 1) // check si le client qui invite et dans le channel
    {
        std::string response = client.returnPrefixe() + ERR_NOTONCHANNEL(channelStr)+ "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    if (channel->checkIfClientOperator(client.getNick())) // check si le client qui invite et un operateur
    {
        std::string response = client.returnPrefixe() + ERR_CHANOPRIVSNEEDED(channelStr) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    std::string nicknameStr = args[1];
    Client * targetClient = serveur.getClient(nicknameStr);
    if (targetClient == NULL) // check si la cible existe dans le serveur
    {
        std::string response = client.returnPrefixe() + ERR_USERNOTINCHANNEL(channel->getChannelName(), nicknameStr) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    if (!(channel->checkIfClientInChannel(serveur.getClient(nicknameStr)))) // check si le client inviter et deja dans le channel
    {
        std::cout <<  "nickbane " << nicknameStr << std::endl;
        std::string response = client.returnPrefixe() + ERR_USERONCHANNEL(targetClient->getNick(), channelStr) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }
    
    // if (channel->getInviteStatus() == true && channel->checkIfClientOperator(&client) != 0) // inutile
	// {
    //     std::string response = client.returnPrefixe() + ERR_CHANOPRIVSNEEDED(channelStr) + "\r\n";
    //     sendResponse(client, serveur, response);
    //     return 1;
    // }
    if (channel->checkIfClientInvited(targetClient) == 1)
        channel->addInvitedClient(targetClient);
    std::string response = client.returnPrefixe() + "INVITE " + nicknameStr + " " + channelStr + "\r\n";
    sendResponse(*targetClient, serveur, response);
    response = client.returnPrefixe() + RPL_INVITING(channelStr, nicknameStr) + "\r\n";
    sendResponse(client, serveur, response);
    return (0);
}
