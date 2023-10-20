#include <string>
#include <ctime>
#include <iostream>
#include <sstream>
#include "../include/Serveur.hpp"

std::string create_prefix_date()
{
    std::time_t currentTime;
    std::time(&currentTime);

    char buffer[80]; // Créez un tampon pour stocker l'heure sous forme de chaîne de caractères
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&currentTime));
    std::string res(buffer);
    return res;
}

std::string create_message(Client client, std::string capted, std::string message)
{
    // Concaténer les chaînes de caractères
    std::string res = create_prefix_date() + capted + "{" + convertIntToString(client.getSocket()) + "}" "[" + message;
    

    //std::cout << create_prefix_date() << capted << "{" << client.getSocket() << "}[" << message << std::endl;
    return res;
}


