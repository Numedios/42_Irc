#include "../include/Serveur.hpp"


std::string getLastWord(const std::string& input) {
    std::istringstream iss(input);

    std::vector<std::string> words;

    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }
    if (!words.empty()) 
        return words.back();
    else
        return "";
}

int handleMode(const std::string& line, Client& client, Serveur& serveur) 
{
    std::vector<std::string> args = createArg(line);
    std::string response;

    
    if (args.size() < 2) 
    {
        response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 
    std::string reason   = createReason(line, 0);
    
    if (args[1][0] == '#' && args.size() > 2) {
        std::string channelStr = args[1];
        Channel* channel = serveur.getChannel(channelStr);
        if (channel->checkIfClientOperator(client.getNick()))
        {
            response = client.returnPrefixe() + ERR_CHANOPRIVSNEEDED(channelStr) + "\r\n";
            sendResponse(client, serveur, response);
            return 1;
        }
        channel->findMode(&client, args[2], args.size() > 3 ? args[3] : "");
    }
    response = client.returnPrefixe() + "MODE #" + reason + "\r\n";
    sendResponse(client, serveur, response);
    return (0);
}