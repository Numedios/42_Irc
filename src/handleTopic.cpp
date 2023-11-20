#include "../include/Serveur.hpp"

std::string concatenateWords(const std::vector<std::string>& args);

int handleTopic(const std::string& line, Client& client, Serveur& serveur)
{
    std::string response;
    std::vector<std::string> args = createArg(line);

    if (args.size() < 2) 
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    std::string channelStr = args[1];
    Channel* channel = serveur.getChannel(channelStr);

    if (args[1][0] != '#' || channel == NULL)
    {
        std::string response = client.returnPrefixe() + ERR_NOSUCHCHANNEL(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    if (args.size() == 2)
    {
        std::string response = TOPIC(channel->getChannelName(), channel->getTopic()) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    if (channel->getTopicStatus() == true) {
        if (channel->checkIfClientOperator(client.getNick()))
        {
            std::string response = client.returnPrefixe() + ERR_CHANOPRIVSNEEDED(channelStr) + "\r\n";
            sendResponse(client, serveur, response);
            return 1;
        }
    }

    //std::cout << "channel name = " << channel->getChannelName() << std::endl;
    if (!channel || channel->checkIfClientInChannel(client.getNick()))
    {
        std::string response = client.returnPrefixe() + ERR_NOTONCHANNEL(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

   // std::string newTopic = args[args.size() - 1];
    args.erase(args.begin(), args.begin() + 2);

    std::string newTopic = concatenateWords(args);
    std::cout << "newtopic = " << newTopic << std::endl;
	if (newTopic.size() == 0)
	{
		channel->setTopic("");
        response = client.returnPrefixe() + TOPIC(channel->getChannelName(), channel->getTopic()) + " :" + "\r\n";
        channel->sendMessageToAll(response, serveur);
	}
    else
    {
        channel->setTopic(newTopic);
        response = client.returnPrefixe() + RPL_TOPIC(channel->getChannelName(), channel->getTopic(), client.getNick()) + "\r\n";
        channel->sendMessageToAll(response, serveur);
    }
    return (0);
}
