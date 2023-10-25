#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <queue>

#include "../include/Client.hpp"
#include "../include/Serveur.hpp"


void sendResponse(Client& client, Serveur& server, const std::string& response) {
    send(client.getSocket(), response.c_str(), response.size(), 0);
    server.addHistoryChat(create_message(client, ":-->   ", response));
}

std::string convertIntToString(int value)
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::vector<std::string> createArg(const std::string& line)
{
    std::istringstream iss(line);
    std::vector<std::string> res;
    std::string word;

    while (iss >> word)
        res.push_back(word);
    return (res);
}


void printVector(std::vector<std::string>& vec) {
    for (size_t i = 0; i < vec.size(); i++) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}