#include "../include/Serveur.hpp"


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
    if (args[1][0] != '#')
    {
        std::string response = client.returnPrefixe() + ERR_NOSUCHCHANNEL(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }
    std::string channelStr = args[1];
    Channel* channel = serveur.getChannel(channelStr);
    if (channel->checkIfClientOperator(client.getNick()))
    {
        std::string response = client.returnPrefixe() + ERR_CHANOPRIVSNEEDED(channelStr) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    //std::cout << "channel name = " << channel->getChannelName() << std::endl;
    if (!channel || channel->checkIfClientInChannel(client.getNick()))
    {
        std::string response = client.returnPrefixe() + ERR_NOTONCHANNEL(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    std::string newTopic = args[args.size() - 1];
    std::map<int, Client *> clients = channel->getClients();
    std::map<int, Client *> operators = channel->getOperators();
	std::map<int, Client *>::iterator it;
	if (newTopic.size() == 0)
	{
		channel->setTopic("");
        response = it->second->returnPrefixe() + TOPIC(channel->getChannelName(), channel->getTopic()) + " :" + "\r\n";
        for (it = clients.begin(); it != clients.end(); it++)
		{
			response = it->second->returnPrefixe() + TOPIC(channel->getChannelName(), channel->getTopic()) + " :" + "\r\n";
            sendResponse(*it->second, serveur, response);
		}
        for (it = operators.begin(); it != operators.end(); it++)
		{
			response = it->second->returnPrefixe() + TOPIC(channel->getChannelName(), channel->getTopic()) + " :" + "\r\n";
            sendResponse(*it->second, serveur, response);
		}
	}
    else
    {
        std::string topic = args[args.size() - 1];
        topic.erase(topic.begin());
        channel->setTopic(topic);
        for (it = clients.begin(); it != clients.end(); it++)
        {
            response = it->second->returnPrefixe() + RPL_TOPIC(channel->getChannelName(), channel->getTopic(), it->second->getNick()) + "\r\n";
            sendResponse(*it->second, serveur, response);
        }
        for (it = operators.begin(); it != operators.end(); it++)
        {
            response = it->second->returnPrefixe() + RPL_TOPIC(channel->getChannelName(), channel->getTopic(), it->second->getNick()) + "\r\n";
            sendResponse(*it->second, serveur, response);
        }
    }
    return (0);
}
