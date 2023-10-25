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


int handleKick(const std::string& line, Client& client, Serveur& serveur)
{
    std::string userkick = getNthWord(line, 2);
    std::string channelName  = getNthWord(line, 1);
    std::string reason   = createReason(line, 2);

    Channel& channel = serveur.getChannel(channelName);

    if (channel.checkIfClientInChannel(userkick) == 0)
    {
        std::string response = client.returnPrefixe() + KICK(channelName, userkick, reason) + "\r\n";
        sendResponse(client, serveur, response);
        std::cout << "la1----------" << std::endl;
        send(serveur.getClient(userkick)->getSocket(), response.c_str(), response.length(), 0);
        std::cout << "la2----------" << std::endl;
        channel.kickClient(userkick);
        std::cout << "la3----------" << std::endl;
        return (0);
    }
    std::string response = client.returnPrefixe() + ERR_NOTONCHANNEL(channelName) + "\r\n";
    sendResponse(client, serveur, response);
    return (1);
}