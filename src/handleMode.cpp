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
    
    // sendResponse(client, serveur, response);

    std::vector<std::string> args = createArg(line);
    std::string response;
    if (args[1][0] == '#') {
        Channel *c = serveur.getChannel(args[1]);
        if(!c->isOperator(&client)) {
            response = "You don't have the correct access to do this\n";
            sendResponse(client, serveur, response);
        }
        c->findMode(&client, args[2], "");
        // response = "RHAAAAAAAAAAAAAAAAAAAA";
        std::string usermode = getLastWord(line);
        response = client.returnPrefixe() + RPL_UMODEIS(client.getNick(), usermode)+ "\r\n";
        sendResponse(client, serveur, response);
    }
    
    return (0);
}