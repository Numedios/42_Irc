// #include "../include/Serveur.hpp"


// int handleTopic(const std::string& line, Client& client, Serveur& serveur)
// {
//     std::string response;
//     std::vector<std::string> args = createArg(line);
//     if (args.size() < 2) 
//     {
//         std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
//         sendResponse(client, serveur, response);
//         return 1;
//     } 
//     std::cout << "args[1] = " << args[1] << std::endl;
//     Channel *channel = serveur.getChannel(args[1]);
//     //std::cout << "channel name = " << channel->getChannelName() << std::endl;
//     if (!channel || channel->checkIfClientInChannel(client.getNick()))
//     {
//         std::string response = client.returnPrefixe() + ERR_NOTONCHANNEL(args[1]) + "\r\n";
//         sendResponse(client, serveur, response);
//         return 1;
//     }

//     std::string newTopic = args[args.size() - 1];
//     std::map<int, Client *> clients = channel->getClients();
//     std::map<int, Client *> operators = channel->getOperators();
// 	std::map<int, Client *>::iterator it;
// 	if (newTopic.size() == 0)
// 	{
// 		channel->setTopic("");
//         response = it->second->returnPrefixe() + TOPIC(channel->getChannelName(), channel->getTopic()) + " :" + "\r\n";
//         for (it = clients.begin(); it != clients.end(); it++)
// 		{
// 			response = it->second->returnPrefixe() + TOPIC(channel->getChannelName(), channel->getTopic()) + " :" + "\r\n";
//             sendResponse(*it->second, serveur, response);
// 		}
//         for (it = operators.begin(); it != operators.end(); it++)
// 		{
// 			response = it->second->returnPrefixe() + TOPIC(channel->getChannelName(), channel->getTopic()) + " :" + "\r\n";
//             sendResponse(*it->second, serveur, response);
// 		}
// 	}
//     else
//     {
//         std::string topic = args[args.size() - 1];
//         topic.erase(topic.begin());
//         channel->setTopic(topic);
//         for (it = clients.begin(); it != clients.end(); it++)
//         {
//             response = it->second->returnPrefixe() + RPL_TOPIC(channel->getChannelName(), channel->getTopic(), it->second->getNick()) + "\r\n";
//             sendResponse(*it->second, serveur, response);
//         }
//         for (it = operators.begin(); it != operators.end(); it++)
//         {
//             response = it->second->returnPrefixe() + RPL_TOPIC(channel->getChannelName(), channel->getTopic(), it->second->getNick()) + "\r\n";
//             sendResponse(*it->second, serveur, response);
//         }
//     }
//     return (0);
// }

#include "../include/Serveur.hpp"

int handleTopic(const std::string& line, Client& client, Serveur& serveur)
{
    std::string response;
    std::vector<std::string> args = createArg(line);
    
    if (args.size() < 2) 
    {
        response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 
    std::cout << "args[1] = " << args[1] << std::endl;
    Channel *channel = serveur.getChannel(args[1]);
    if (!channel || channel->checkIfClientInChannel(&client))
    {
        response = client.returnPrefixe() + ERR_NOTONCHANNEL(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    // Check if the client is an operator or if the channel topic is not locked.
    if (channel->isTopicSetOnlyByOp() && !channel->isOperator(&client))
    {
        response = client.returnPrefixe() + ERR_CHANOPRIVSNEEDED(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    // If new topic is empty, it means the client wants to see the current topic.
    if (args.size() == 2)
    {
        response = client.returnPrefixe() + RPL_TOPIC(channel->getChannelName(), channel->getTopic()) + "\r\n";
        sendResponse(client, serveur, response);
    }
    else
    {
        // Set the new topic.
        std::string topic = line.substr(line.find(args[2]));
        channel->setTopic(topic);
        response = ":" + client.getNick() + "!" + client.getUserName() + "@" + client.getHostName() + " TOPIC " + channel->getChannelName() + " :" + topic + "\r\n";
        
        // Notify all clients in the channel about the topic change.
        std::map<int, Client *> clients = channel->getClients();
        std::map<int, Client *> operators = channel->getOperators();
        for (auto &clientPair : clients)
        {
            sendResponse(*clientPair.second, serveur, response);
        }
        for (auto &operatorPair : operators)
        {
            sendResponse(*operatorPair.second, serveur, response);
        }
    }
    return 0;
}
