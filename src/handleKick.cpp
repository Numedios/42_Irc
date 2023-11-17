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


int handleKick(const std::string& line, Client& client, Serveur& serveur)
{

    std::vector<std::string> args = createArg(line);
    std::string response;

    if (args.size() < 4) 
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 
    std::string channelName = args[1];
    Channel* channelParse = serveur.getChannel(channelName);
    if (channelParse->checkIfClientOperator(client.getNick()))
    {
        response = client.returnPrefixe() + ERR_CHANOPRIVSNEEDED(channelName) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    std::string userkick = args[2];
    std::string reason   = createReason(line, 2);
    Channel& channel = *serveur.getChannel(channelName);
    std::vector<std::string> channels_name = multipleParams(channelName);

    // check il y a un mask = #
    for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); ++it)
	{
		if ((*it)[0] != '#')
        {
            response = serveur.getClient(userkick)->returnPrefixe() + ERR_BADCHANMASK(*it) + "\r\n";
            sendResponse(client, serveur, response);
        	return 1;
        }
    }

    // check si le channel existe
    for (std::vector<std::string>::iterator it = channels_name.begin(); it != channels_name.end(); ++it)
	{
		if (serveur.checkChannelInServeur(*it) == 1)
        {
            response = client.returnPrefixe() + ERR_NOSUCHCHANNEL(*it) + "\r\n";
            sendResponse(client, serveur, response);
        	return 1;
        }
	}

	// check autoban
    if (client.getNick() == userkick) {
        std::string response = client.returnPrefixe() + ERR_NICKNAMEINUSE(client.getNick()) + "\r\n";
        sendResponse(client, serveur, response);
        return (1);
    }

    if (channel.checkIfClientInChannel(userkick) == 0)
    {
        response = serveur.getClient(userkick)->returnPrefixe() + "PART " + channel.getChannelName() + " " + channel.getChannelName() + " :kicked" +  "\r\n";
        channel.sendMessageToAll(response, serveur);
        send(serveur.getClient(userkick)->getSocket(), response.c_str(), response.length(), 0);
        serveur.addHistoryChat(response);
        //response = client.returnPrefixe() + KICK(channelName, userkick, reason) + "\r\n";
        //channel.sendMessageToAll(response, serveur);
        //sendResponse(client, serveur, response);
        channel.kickClient(userkick);
        return (0);
    }
    response = client.returnPrefixe() + ERR_USERNOTINCHANNEL(channel.getChannelName(), userkick) + "\r\n";
    sendResponse(client, serveur, response);
    return (1);
}


// PART #lol :kicked
