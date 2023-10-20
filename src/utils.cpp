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