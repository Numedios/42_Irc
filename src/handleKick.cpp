#include "../include/Serveur.hpp"

std::string createReason(const std::string& line, int place)
{
    std::istringstream iss(line);
    std::string word;
    std::string reason = "";
    int i = 0;

    while (iss >> word)
    {
        if (i > place)
        {
            if (i == place + 1)
                reason += word.substr(1) + " ";
            else
                reason += word + " ";
        }
        i++;
    }
    return reason.substr(0, reason.length() - 1);
}

std::string createReasonMode(const std::string& line, int place)
{
    std::istringstream iss(line);
    std::string word;
    std::string reason = "";
    int i = 0;

    while (iss >> word)
    {
        if (i > place)
        {
            if (i == place + 1)
                reason += word + " ";
            else
                reason += word + " ";
        }
        i++;
    }
    return reason.substr(0, reason.length() - 1);
}


std::vector<std::string> multipleParams(std::string params)
{
	std::vector<std::string> result;
	size_t limit;
	size_t it = 0;
	while(it < params.size())
	{
		limit = params.find(',', it);
		if (limit == params.npos)
			limit = params.size();
		result.push_back(params.substr(it, limit - it));
		it = limit + 1;
	}
	return result;
}

/*

args[0] = KICK
args[1] = nom du userkick
args[2] = channel
args[3 et +] = reason du kick 

*/


int handleKick(const std::string& line, Client& client, Serveur& serveur)
{

    std::vector<std::string> args = createArg(line);
    std::string response;

    if (args.size() < 4)  // 1
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 
    std::string channelName = args[1];
    std::string userkick = args[2];

    Channel* channelParse = serveur.getChannel(channelName);

    if (args[1][0] != '#' || channelParse == NULL) // 2
    {
        std::string response = client.returnPrefixe() + ERR_NOSUCHCHANNEL(args[1]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    if (channelParse->checkIfClientInChannel(client.getNick()) == 1) // 3
    {
        std::string response = client.returnPrefixe() + ERR_NOTONCHANNEL(channelName)+ "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    if (channelParse->checkIfClientOperator(client.getNick())) // 4
    {
        response = client.returnPrefixe() + ERR_CHANOPRIVSNEEDED(channelName) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }


    Client * targetClient = serveur.getClient(userkick);
    std::string reason   = createReason(line, 2);
    Channel& channel = *serveur.getChannel(channelName);
    std::vector<std::string> channels_name = multipleParams(channelName);

    if (targetClient == NULL) // 5 check si la cible existe dans le serveur
    {
        response = client.returnPrefixe() + ERR_USERNOTINCHANNEL(channel.getChannelName(), userkick) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }




	// check autoban
    if (client.getNick() == userkick) // 6 check si c'est un autoban 
    { 
        if (channel.checkIfClientOperator(client.getNick()) == 0 && channel.getOperators().size() == 1)
        {
            if (!channel.getClients().empty())
            {
                std::map<int, Client *>::iterator it = channel.getClients().begin();
                Client * tmp = it->second;
                channel.setOperator(tmp);
                channel.delClient(tmp);
            }
        }
    }

    if (channel.checkIfClientInChannel(userkick) == 0) // 7
    {
        response = serveur.getClient(userkick)->returnPrefixe() + "PART " + " " + channel.getChannelName() + " :kicked" +  "\r\n";
        channel.sendMessageToAll(response, serveur);
        send(serveur.getClient(userkick)->getSocket(), response.c_str(), response.length(), 0);
        serveur.addHistoryChat(response);
        channel.kickClient(userkick);
        return (0);
    }
    // response = client.returnPrefixe() + ERR_USERNOTINCHANNEL(channel.getChannelName(), userkick) + "\r\n"; 
    // sendResponse(client, serveur, response);
    return (1);
}


// PART #lol :kicked
