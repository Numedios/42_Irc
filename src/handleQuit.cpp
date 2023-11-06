
#include "../include/Serveur.hpp"

int handleQuit(const std::string& line, Client& client, Serveur& serveur) {
    std::vector<std::string> args = createArg(line);
    std::string reason = createReason(line, 0);
    

    serveur.handleClientDisconnect(client.getId(), reason);
    return 0;
}

