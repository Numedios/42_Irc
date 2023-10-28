
#include "../include/Serveur.hpp"

int handleQuit(const std::string& line, Client& client, Serveur& serveur) {
    std::vector<std::string> args = createArg(line);
    std::string reason = createReason(line, 0);

    std::string response = client.returnPrefixe() + "QUIT :" + reason + "\r\n";
    sendResponse(client, serveur, response);
    return 0;
}

