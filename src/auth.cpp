#include "../include/Serveur.hpp"
// //#include "client.cpp"
// #include "serveur.cpp"
// //#include "../include/Command.hpp"
// //#include "../include/Responses.hpp"
#include <sstream>
#include <iostream>
#include <string>
#include <ctime>
#include <map>
#include <functional>


// class Serveur; // Déclaration avancée (forward declaration)
// class Client;
// // class Command;

// //typedef int (*FunctionPtr)(const std::string&, Client&, Serveur&);

// /*
//     check le bytes send sur tous les send()
//      if (bytesSent == -1) 
//             std::cout << "failure" << std::endl;r.
//         else
//             std::cout << "success" << std::endl;
// */

int handleCapLS(const std::string& line, Client& client, Serveur& serveur) 
{
    if (client.getAuthState() == Client::NotAuthenticated) {
        std::ostringstream oss;
        oss << "CAP * LS :" << MAX_CLIENTS << "\n";
        std::string response = oss.str();
        if (client.getId() > MAX_CLIENTS) {
            response = ":Server ERROR : Server is full, please try again later\r\n";
            //std::cout << "nb client = " << client.getId() << " // max client = " << MAX_CLIENTS << std::endl;
            sendResponse(client, serveur, response);
            return (1);
        }
        serveur.addHistoryChat(create_message(client, ":-->   ", response));
        // ssize_t bytesSent = send(client.getSocket(), response.c_str(), response.size(), 0);
        // if (bytesSent == -1) 
        //     std::cout << "failure" << std::endl;
        // else
        //     std::cout << "success" << std::endl;
        client.setAuthState(Client::CapLs);
    }
    return (0);
}



int handlePass(const std::string& line, Client& client, Serveur& serveur)
{
    if (client.getAuthState() == Client::CapLs) {
        std::istringstream iss(line);
        std::string word;
        std::string target = "PASS";

        while (iss >> word) {
            if (word == target) {
                if (iss >> word) {
                    if (word == serveur.getPassword())
                    {
                        //std::cout << "pass VALID" << std::endl;
                    }
                    else 
                    {
                        std::cout << "Error : pass INVALID" << std::endl;
                        std::string response = "ERROR :Password incorrect. Closing connection.\r\n";
                        sendResponse(client, serveur, response);
                        client.setDisconnected();
                        client.setAuthState(Client::NotAuthenticated);
                        return 1;
                    }
                }
            }
        }
        client.setAuthState(Client::Pass);
    }
    return (0);
}



int handleNick(const std::string& line, Client& client, Serveur& serveur) 
{
    std::cout << "" << std::endl;
    if (client.getAuthState() == Client::Pass) {
        std::istringstream iss(line);
        std::string word;
        std::string target = "NICK";

        while (iss >> word) {
            if (word == target) {
                if (iss >> word) {
                    bool nickTaken = false;
                    std::map<int , Client *> clients = serveur.getClients();


                    for (size_t i = 1; i < clients.size(); ++i) {
                        Client* otherClient = clients[i];
                        if (otherClient != &client && otherClient->getNick() == word) {
                            std::string response = ":Server ERROR : Nickname is already in use. Please choose another nickname.\r\n";
                            sendResponse(client, serveur, response);
                            // Fermez la connexion du client
                            client.setAuthState(Client::NotAuthenticated); // sa se discute
                            client.setDisconnected();
                            nickTaken = true;
                            break;
                        }
                    }
                    if (nickTaken) {
                        client.setAuthState(Client::NotAuthenticated);
                        return 1;
                    }
                    std::string response = ": NICK :" + word + "\r\n";
                    client.setNick(word);
                    sendResponse(client, serveur, response);
                    serveur.addHistoryChat(create_message(client, ":SERVER", client.getNick() + "(" +  convertIntToString(client.getSocket()) + ")" + " has been connected.\n"));
                    client.setAuthState(Client::Nick);
                    return 0;
                }
            }
        }
        std::cout << "No nickname found after 'NICK'.";
    }
    return 0;
}




std::string extractUsername(const std::string& line) {
    size_t userPos = line.find("USER");
    if (userPos != std::string::npos) {
        size_t afterUserPos = line.find_first_not_of(" ", userPos + 4);
        if (afterUserPos != std::string::npos) {
            size_t endWordPos = line.find_first_of(" \r\n\0", afterUserPos);
            if (endWordPos == std::string::npos) {
                endWordPos = line.size();
            }
            return line.substr(afterUserPos, endWordPos - afterUserPos);
        }
    }
    return ""; // Return an empty string if USER information is not found
}

int handleUser(const std::string& line, Client& client, Serveur& serveur) {
    if (client.getAuthState() == Client::Nick) {
        size_t userPos = line.find("USER");
        if (userPos != std::string::npos) {
            size_t afterUserPos = line.find_first_not_of(" ", userPos + 4);
            if (afterUserPos != std::string::npos) {
                size_t endWordPos = line.find_first_of("\r\n\0", afterUserPos);
                if (endWordPos == std::string::npos) {
                    endWordPos = line.size();
                }
                std::string word = client.getNick();
                std::string userName = extractUsername(line);
                client.setUserName(userName);
                std::string response = client.returnPrefixe() + RPL_WELCOME(word, userName, "@127.0.0.1") + "\r\n";
                sendResponse(client, serveur, response);
            }
        } else {
            std::cout << "No 'USER' found in the line." << std::endl;
            std::string response = ":Server ERROR : No USER information provided. Please provide valid USER information.\r\n";
            sendResponse(client, serveur, response);
            return 0;
        }
        client.setAuthState(Client::Authenticated);
        client.setAuthenticated(true);
    }
    return 0;
}

void fillCommandMapAuth(std::map<std::string, FunctionPtr>& commands)
{
    commands["CAP"] = handleCapLS;
    commands["PASS"] =   handlePass;
    commands["NICK"] =   handleNick;
    commands["USER"] =   handleUser;
}



// /*

// /rawlog save nomficheir.txt

// */