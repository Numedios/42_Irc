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
    std::cout << "args[1] = " << args[1] << std::endl;
    Channel *channel = serveur.getChannel(args[1]);
    //std::cout << "channel name = " << channel->getChannelName() << std::endl;
    if (!channel || channel->checkIfClientInChannel(client.getNick()))
    {
        std::string response = client.returnPrefixe() + ERR_NOTONCHANNEL(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    std::string newTopic = args[args.size() - 1];
    std::map<int, Client *> clients = channel->getClients();
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
        response = channel->getOperator()->returnPrefixe() + TOPIC(channel->getChannelName(), channel->getTopic()) + " :" + "\r\n";
        sendResponse(*channel->getOperator(), serveur, response);
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
        response = channel->getOperator()->returnPrefixe()  + RPL_TOPIC(channel->getChannelName(), channel->getTopic(), channel->getOperator()->getNick()) + "\r\n";
        sendResponse(*channel->getOperator(), serveur, response);
    }
    return (0);
}


/*
15:59:47:-->   {4}[:said!sbelabba@127.0.0.1 332 #lol :onestla
15:56:17:-->   {4}[:sofiane!sbelabba@127.0.0.1 332 sofiane #lol :on est la

15:56:17:-->   {5}[:said!sbelabba@127.0.0.1 332 said #lol :on est la
15:59:47:-->   {5}[:sofiane!sbelabba@127.0.0.1 332 #lol :onestla



*/