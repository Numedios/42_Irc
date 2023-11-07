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
    std::string usermode = getLastWord(line);
    std::string response = client.returnPrefixe() + RPL_UMODEIS(client.getNick(), usermode)+ "\r\n";
    
    sendResponse(client, serveur, response);

    
    
    return (0);
}