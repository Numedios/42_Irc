#include "../include/Serveur.hpp"

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
    (void) line;
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
        client.setAuthState(Client::CapLs);
    }
    return (0);
}

int handlePass(const std::string& line, Client& client, Serveur& serveur)
{  
    std::vector<std::string> args = createArg(line);

    if (client.isAuthenticated()  == true || client.getAuthState() != Client::CapLs)
    {
        std::string response = client.returnPrefixe() + ERR_ALREADYREGISTRED() + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    if (args.size() < 2) 
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 

    if (args[1] != serveur.getPassword())
    {
        std::cout << "Error : pass INVALID" << std::endl;
        std::string response = client.returnPrefixe() + ERR_PASSWDMISMATCH() + "\r\n";
        sendResponse(client, serveur, response);
        client.setDisconnected();
        client.setAuthState(Client::NotAuthenticated);
        return 1;
    }
    client.setAuthState(Client::Pass);

    return (0);
}

// special = %x5B-60 / %x7B-7D ; "[", "]", "\", "`", "_", "^", "{", "|", "}"
bool	isSpecial(char c) {
	if (c == '[' || c == ']' || c == '\\' || c == '`'
		|| c == '_' || c == '^' || c == '{' || c == '|' || c == '}')
		return true;

	return false;
}

// letter = %x41-5A / %x61-7A ; A-Z / a-z
bool	isLetter(char c) {
	if (c >= 'A' && c <= 'Z')
		return true;

	if (c >= 'a' && c <= 'z')
		return true;

	return false;
}

// digit = %x30-39 ; 0-9
bool	isDigit(char c) {
	if (c >= '0' && c <= '9')
		return true;

	return false;
}

static bool	isNicknameProperlyFormatted(std::string nickname) {
	size_t	i = 0;

	if (nickname.size() > 9)
		return false;

	while (i < nickname.size()) {
		if (i < 8) {
			if (!isLetter(nickname[i]) && !isSpecial(nickname[i]))
				return false;
		}
		else
			if (!isLetter(nickname[i]) && !isDigit(nickname[i])
				&& !isSpecial(nickname[i]) && nickname[i] != '-')
				return false;

		i++;
	}
	return true;
}

static bool	isNicknameAlreadyInUse(Serveur& server, std::string nickname) {
	std::map<int, Client*>::iterator it = server.getClients().begin();

	for (; it != server.getClients().end(); it++)
    {
		if (it->second->getNick() == nickname)
        {
			return true;
        }
    }
	return false;
}

int handleNick(const std::string& line, Client& client, Serveur& serveur) 
{
    std::vector<std::string> args = createArg(line);
    std::string response;
    if (client.isAuthenticated()  != true && client.getAuthState() != Client::Pass)
    {
        return 1;
    }
    if (args.size() < 2) 
    {
        response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }
    std::string nickname = args[1];
	if (!isNicknameProperlyFormatted(nickname))
    {
        response = client.returnPrefixe() + ERR_ERRONEUSNICKNAME(nickname) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }
    if (isNicknameAlreadyInUse(serveur, nickname)) {
        response = client.returnPrefixe() + ERR_NICKNAMEINUSE(nickname) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
	}
    if (client.isAuthenticated()  != true)
        response = ": NICK :" + args[1] + "\r\n";
    else 
        response = client.returnPrefixe() + " NICK :" + args[1] + "\r\n";
    client.setNick(args[1]);
    sendResponse(client, serveur, response);
    if (client.isAuthenticated()  != true)
    {
        serveur.addHistoryChat(create_message(client, ":SERVER", client.getNick() + "(" +  convertIntToString(client.getSocket()) + ")" + " has been connected.\n"));
        client.setAuthState(Client::Nick);
    }
    return 0;
}

// int handleNick(const std::string& line, Client& client, Serveur& serveur) 
// {
//     if (client.getAuthState() == Client::Pass) {
//         std::istringstream iss(line);
//         std::string word;
//         std::string target = "NICK";
//         while (iss >> word) {
//             if (word == target) {
//                 if (iss >> word) {
//                     bool nickTaken = false;
//                     std::map<int , Client *> clients = serveur.getClients();

//                     for (size_t i = 1; i < clients.size(); ++i) {
//                         Client* otherClient = serveur.getClient(serveur.getFdsSocket(i));
//                         if (otherClient != &client && otherClient->getNick() == word) {
//                             std::string response = ":Server ERROR : Nickname is already in use. Please choose another nickname.\r\n";
//                             sendResponse(client, serveur, response);
//                             // Fermez la connexion du client
//                             client.setAuthState(Client::NotAuthenticated); // sa se discute
//                             client.setDisconnected();
//                             nickTaken = true;
//                             break;
//                         }
//                     }
//                     if (nickTaken) {
//                         client.setAuthState(Client::NotAuthenticated);
//                         return 1;
//                     }
//                     std::string response = ": NICK :" + word + "\r\n";
//                     client.setNick(word);
//                     sendResponse(client, serveur, response);
//                     serveur.addHistoryChat(create_message(client, ":SERVER", client.getNick() + "(" +  convertIntToString(client.getSocket()) + ")" + " has been connected.\n"));
//                     client.setAuthState(Client::Nick);
//                     return 0;
//                 }
//             }
//         }
//         std::cout << "No nickname found after 'NICK'.";
//     }
//     return 0;
// }


int handleUser(const std::string& line, Client& client, Serveur& serveur) {
    std::vector<std::string> args = createArg(line);


    if (client.isAuthenticated()  == true || client.getAuthState() != Client::Nick)
    {
        std::string response = client.returnPrefixe() + ERR_ALREADYREGISTRED() + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    }

    if (args.size() < 5) 
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 
        client.setUserName(args[1]);
        std::string response = client.returnPrefixe() + RPL_WELCOME(client.getNick(), args[1], "127.0.0.1") + "\r\n";
        sendResponse(client, serveur, response);

        client.setAuthState(Client::Authenticated);
        client.setAuthenticated(true);
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