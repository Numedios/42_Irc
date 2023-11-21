#include "../include/Serveur.hpp"

void joinChannel(Channel& channel, Client& client, Serveur& serveur, std::string key)
{
    std::string response;
    std::string nameChannel = channel.getChannelName();
    
    if (channel.getNumberClient() + 1 > channel.getMaxUsers()) // check limite client in channel (Pas test)
    {
        response = client.returnPrefixe() + ERR_CHANNELISFULL(client.getNick(), nameChannel) + "\r\n";
        sendResponse(client, serveur, response);
        return;
    } 
    if (!channel.getKey().empty() && channel.getKey() != key) // check si une clef existe et si elle corespond
    {
        response = client.returnPrefixe() + ERR_BADCHANNELKEY(client.getNick(), channel.getChannelName()) + "\r\n";
        sendResponse(client, serveur, response);
        return;
    }
    if (channel.getInviteStatus() == true) // si actuellement en mode +i
    {
        if ( channel.checkIfClientInvited(client.getNick()) == 1) // si le client ne fait pas partie des inviter
        {
            response = client.returnPrefixe() + ERR_INVITEONLYCHAN(client.getNick(), nameChannel) + "\r\n";
            sendResponse(client, serveur, response);
            return ;
        }
        else // si il fait partie des inviter
            channel.delInvitedClient(&client);
    }
    if (channel.getOperators().empty()) // si il n'y a pas d'operateur dans le channel il le devient (inutile)
        channel.setOperator(&client);
    else
        channel.addClient(&client);
    
    response = client.returnPrefixe() + RPL_NAMREPLY(nameChannel, client.getNick(), channel.sendAllClientsNames()) + "\r\n";
    sendResponse(client, serveur, response);

    response = client.returnPrefixe() + RPL_ENDOFNAMES(nameChannel, client.getNick()) + "\r\n";
    sendResponse(client, serveur, response);

    response = client.returnPrefixe() + "JOIN " + nameChannel + "\r\n";
    channel.sendMessageToAll(response, serveur);

    if (!channel.getTopic().empty())
    {
        response = client.returnPrefixe() + RPL_TOPIC(channel.getChannelName(), channel.getTopic(), client.getNick()) + "\r\n";
        sendResponse(client, serveur, response);
    }
}


void createChannel(const std::string& nameChannel, Client& client, Serveur& serveur)
{
    Channel channel = Channel(nameChannel);

    Client *clientPtr = &client;
    channel.setOperator(clientPtr);
    serveur.addChannel(nameChannel, channel);

    std::string response;

    response = client.returnPrefixe() + RPL_NAMREPLY(nameChannel, client.getNick(), channel.sendClientName(client)) + "\r\n";
    sendResponse(client, serveur, response);

    response = client.returnPrefixe() + RPL_ENDOFNAMES(nameChannel, client.getNick()) + "\r\n";
    sendResponse(client, serveur, response);

    response = client.returnPrefixe() + "JOIN " + nameChannel + "\r\n";
    sendResponse(client, serveur, response);
}


/*

args[0] = JOIN
args[1] = #nameChannel
args[2] = key (optionnel)

*/

int handleJoin(const std::string& line, Client& client, Serveur& serveur)
{
    std::vector<std::string> args = createArg(line);
    std::string response;

    if (args.size() < 2) // nombre arguments
    {
        response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 

    std::string nameChannel = args[1];

    if (nameChannel.empty() || nameChannel.size() == 1 ||  nameChannel[0] != '#') // debut avec # ou vide
    {
        response = client.returnPrefixe() + ERR_BADCHANMASK(nameChannel) + "\r\n";
        sendResponse(client, serveur, response);
        return (1);
    }

    Channel*  channel = serveur.getChannel(nameChannel);
    if (channel != NULL) // si le serveur n'existe pas
    {

        std::cout << client.getNick() << " join channel :" << nameChannel << std::endl;
        if (args.size() > 2)  // presence de la key ou non 
            joinChannel(*serveur.getChannel(nameChannel), client, serveur, args[2]);
        else 
            joinChannel(*serveur.getChannel(nameChannel), client, serveur, "");
        return (1);
    }
    std::cout << client.getNick() << " create new channel :" << nameChannel << std::endl;
    createChannel(nameChannel, client, serveur);
    return (0);
}