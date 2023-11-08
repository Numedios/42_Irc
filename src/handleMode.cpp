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
    if (args[0] != "MODE")
        return (0);
    if (args[1][0] == '#' && args.size() > 2) {
        Channel *c = serveur.getChannel(args[1]);
        if(!c->isOperator(&client)) {
            response = "You don't have the correct access to do this\n";
            sendResponse(client, serveur, response);
        }
        c->findMode(&client, args[2], args.size() > 3 ? args[3] : "");
    }    
    return (0);
}