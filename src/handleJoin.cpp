#include "../include/Serveur.hpp"

void joinChannel(Channel& channel, Client& client, Serveur& serveur, std::string key)
{
    std::string response;
    std::string name = channel.getChannelName();
    
    if (channel.getClients().size() == (size_t) channel.getMaxUsers())
    {
        response = client.returnPrefixe() + ERR_CHANNELISFULL(client.getNick(), name) + "\r\n";
        sendResponse(client, serveur, response);
        return;
    }
    if (channel.getKey() != key) {
        std::cout <<" key = " << channel.getKey() << std::endl;
        std::cout << key << std::endl;
        response = client.returnPrefixe() + ERR_BADCHANNELKEY(client.getNick(), channel.getChannelName()) + "\r\n";
        sendResponse(client, serveur, response);
        return;
    }

    if (channel.getInviteStatus() == true)
    {
        if ( channel.checkIfClientInvited(client.getNick()) == 1)
        {
            response = client.returnPrefixe() + ERR_INVITEONLYCHAN(client.getNick(), name) + "\r\n";
            sendResponse(client, serveur, response);
            return ;
        }
        else
            channel.delInvitedClient(&client);
    }
    if (channel.getOperators().empty())
        channel.setOperator(&client);
    else
        channel.addClient(&client);
    
    response = client.returnPrefixe() + RPL_NAMREPLY(name, client.getNick(), channel.sendAllClientsNames()) + "\r\n";
    sendResponse(client, serveur, response);

    response = client.returnPrefixe() + RPL_ENDOFNAMES(name, client.getNick()) + "\r\n";
    sendResponse(client, serveur, response);

    response = client.returnPrefixe() + "JOIN " + name + "\r\n";
    sendResponse(client, serveur, response);
}


void createChannel(const std::string& name, Client& client, Serveur& serveur)
{
    Channel channel = Channel(name);

    Client *clientPtr = &client;
    channel.setOperator(clientPtr);
    serveur.addChannel(name, channel);

    std::string response;

    response = client.returnPrefixe() + RPL_NAMREPLY(name, client.getNick(), channel.sendClientName(client)) + "\r\n";
    sendResponse(client, serveur, response);

    response = client.returnPrefixe() + RPL_ENDOFNAMES(name, client.getNick()) + "\r\n";
    sendResponse(client, serveur, response);

    response = client.returnPrefixe() + "JOIN " + name + "\r\n";
    sendResponse(client, serveur, response);
}

int handleJoin(const std::string& line, Client& client, Serveur& serveur)
{
    std::vector<std::string> args = createArg(line);
    if (args.size() < 2) 
    {
        std::string response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 
    std::string name = args[1];

    if (name.empty())
    {
        serveur.addHistoryChat(create_message(client, ":->test", "Error : pas de nom a join\n"));
        return (0);
    }

    std::map<std::string, Channel>& channels = serveur.getChannels();
    if (channels.find(name) != channels.end())
    {

        std::cout << client.getNick() << " join channel :" << name << std::endl;
        if (args.size() > 2) 
            joinChannel(*serveur.getChannel(name), client, serveur, args[2]);
        else 
            joinChannel(*serveur.getChannel(name), client, serveur, "");
        return (1);
    }
    std::cout << client.getNick() << " create new channel :" << name << std::endl;
    createChannel(name, client, serveur);
    return (0);
}


/*

rajouter la gestion de key dans join

*/


/*

:said!sbelabba@127.0.0.1 473 #lol :Cannot join channel (+i)
:said!sbelabba@127.0.0.1 473 said #lol :Cannot join channel (+i)




*/
