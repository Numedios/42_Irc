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


/*

    std::cout << "Undefined mode" << std::endl;
    response = client->returnPrefixe() + ERR_INVALIDMODEPARAM(client->getNick(), client->getNick() + "@localhost" , args[1], args[2], "mode parameter needed") + "\r\n";
    sendResponse(*client, serveur, response);
    response = client->returnPrefixe() + ERR_INVALIDMODEPARAM(client->getNick(), client->getNick() + "@localhost" , args[1], args[2], "invalid number of mode parameters") + "\r\n";
    sendResponse(*client, serveur, response);
    return (1);

*/

int    findMode(Client *client, std::vector<std::string> args, Serveur& serveur,Channel& channel) {
    std::map<std::string, ModeFunction>::iterator it = channel.getMods().find(args[2]);
    std::string response;

    if (it != channel.getMods().end()) {
        ModeFunction func = it->second;
        (func)(args, *client, serveur, channel);
    } else {
        std::cout << "Undefined mode" << std::endl;
        response = client->returnPrefixe() + ERR_UNKNOWNMODE(args[2], args[1]) + "\r\n";
        sendResponse(*client, serveur, response);
        return (1);
    }
    return (0);
}

int    setInvitation(std::vector<std::string>& args, Client& client, Serveur& serveur, Channel& channel) 
{
    channel.setInviteStatus(true);
    std::cout << "Serveur is now on invitation mode" << std::endl;
    (void) args;
    (void) client;
    (void)(serveur);
    return (0);
}


int   removeInvitation(std::vector<std::string>& args, Client& client, Serveur& serveur, Channel& channel) 
{
    std::cout << "Invitation mode has been removed" << std::endl;
    channel.setInviteStatus(false);
    (void) serveur;
    (void) channel;
    (void) args;
    (void) client;
    return (0);
}


int    setKeyPass(std::vector<std::string>& args, Client& client, Serveur& serveur, Channel& channel) {
    if (args.size() < 4 || args[3].empty()) {
        std::cout << "Uncorrect value of arguments" << std::endl;
        return 1;
    }
    channel.setKey(args[3]);
    std::cout << "Password has been updated" << std::endl;
    (void) serveur;
    (void) channel;
    (void) args;
    (void) client;
    return 0;
}

std::string concatenateWords(const std::vector<std::string>& args) {
    std::string result;

    for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
        result += *it;
        if (it + 1 != args.end()) {
            result += " ";
        }
    }

    return result;
}


int    removeKeyPass(std::vector<std::string>& args, Client& client, Serveur& serveur, Channel& channel)
{
    channel.setKey("");
    std::cout << "Password has ben removed" << std::endl;
    (void) client;
    (void) serveur;
    (void) channel;
    (void) args;
    return 0;
}

int     addOperator(std::vector<std::string>& args, Client& client, Serveur& serveur, Channel& channel)
{
    std::string response;
    if (args.size() < 4 || args[3].empty()) {
        response = client.returnPrefixe() + ERR_INVALIDMODEPARAM(client.getNick(), client.getNick() + "@localhost" , args[1], args[2], "mode parameter needed") + "\r\n";
        sendResponse(client, serveur, response);
        response = client.returnPrefixe() + ERR_INVALIDMODEPARAM(client.getNick(), client.getNick() + "@localhost" , args[1], args[2], "invalid number of mode parameters") + "\r\n";
        sendResponse(client, serveur, response);
        return (1);
    }
    Client * target = serveur.getClient(args[3]);
    if (target == NULL)
    {
        response = client.returnPrefixe() + ERR_NOSUCHNICK(args[3]) + "\r\n";
        sendResponse(client, serveur, response);
        return (1);
    }
    if (channel.getClient(target->getNick()) == NULL)
    {
        response = client.returnPrefixe() + ERR_USERNOTINCHANNEL(args[3], channel.getChannelName()) + "\r\n";
        sendResponse(client, serveur, response);
        return (1);
    }
    if ((channel.isOperator(target)))
    {
        response = client.returnPrefixe() +  ERR_INVALIDMODEPARAM(client.getNick(), client.getNick() + "@localhost", args[2], channel.getChannelName(), "is already channel operator") + "\r\n";
        sendResponse(client, serveur, response);
        return (1);
    }
    channel.delClient(&client);
    channel.setOperator(&client);
    response = client.returnPrefixe() + concatenateWords(args) + "\r\n";
    channel.sendMessageToAll(response, serveur);
    std::cout << target->getNick() << " is now an operator" << std::endl;
    (void) client;
    (void) serveur;
    (void) channel;
    (void) args;
    return 0;
}

int    removeOperator(std::vector<std::string>& args, Client& client, Serveur& serveur, Channel& channel)
{
    std::string response;
    if (args.size() < 4 || args[3].empty()) {
        response = client.returnPrefixe() + ERR_INVALIDMODEPARAM(client.getNick(), client.getNick() + "@localhost" , args[1], args[2], "mode parameter needed") + "\r\n";
        sendResponse(client, serveur, response);
        response = client.returnPrefixe() + ERR_INVALIDMODEPARAM(client.getNick(), client.getNick() + "@localhost" , args[1], args[2], "invalid number of mode parameters") + "\r\n";
        sendResponse(client, serveur, response);
        return (1);
    }
    Client * target = serveur.getClient(args[3]);
    if (target == NULL)
    {
        response = client.returnPrefixe() + ERR_NOSUCHNICK(args[3]) + "\r\n";
        sendResponse(client, serveur, response);
        return (1);
    }
    if (channel.getClient(target->getNick()) == NULL)
    {
        response = client.returnPrefixe() + ERR_USERNOTINCHANNEL(args[3], channel.getChannelName()) + "\r\n";
        sendResponse(client, serveur, response);
        return (1);
    }
    if (!(channel.isOperator(target)))
    {
        std::string response = client.returnPrefixe() +  ERR_INVALIDMODEPARAM(client.getNick(), client.getNick() + "@localhost", args[2], channel.getChannelName(), "is no channel operator") + "\r\n";
        sendResponse(client, serveur, response);
        return (1);
    }
    if (channel.getOperators().size() == 1)
    {
        std::string response = client.returnPrefixe() +  ERR_INVALIDMODEPARAM(client.getNick(), client.getNick() + "@localhost", args[2], channel.getChannelName(), "you cannot demote yourself, you are the only operator left on this chan") + "\r\n";
        sendResponse(client, serveur, response);
        return (1);
    }
    channel.delOperator(&client);
    channel.addClient(&client);
    channel.sendMessageToAll(response, serveur);
    std::cout << (&client)->getNick() << " is no more an operator" << std::endl;
    (void) client;
    (void) serveur;
    (void) channel;
    (void) args;
    return 0;
}



int   setLimitUser(std::vector<std::string>& args, Client& client, Serveur& serveur, Channel& channel) 
{
    if (args.size() < 3 || args[3].empty()) {
        std::cout << "Uncorrect value limit" << std::endl;
        return 1;
    }
    channel.setMaxUsers(std::atoi(args[3].c_str()));
    std::cout << "Channel limit is now " << args[3] << std::endl;
    (void) client;
    (void) serveur;
    (void) channel;
    (void) args;
    return 0;
}

int    removeLimitUser(std::vector<std::string>& args, Client& client, Serveur& serveur, Channel& channel) 
{
    channel.setMaxUsers(MAX_CLIENTS);
    std::cout << "Channel limit has been removed" << std::endl;
    (void) client;
    (void) serveur;
    (void) channel;
    (void) args;
    return 0;
}

int setTopicProtection(std::vector<std::string>& args, Client& client, Serveur& serveur, Channel& channel)
{
    channel.setTopicProtection(true);
    std::cout << "Topic protection is now enabled" << std::endl;
    (void) client;
    (void) serveur;
    (void) channel;
    (void) args;
    return 0;
}

int removeTopicProtection(std::vector<std::string>& args, Client& client, Serveur& serveur, Channel& channel) 
{
    channel.setTopicProtection(true);
    std::cout << "Topic protection is now disabled" << std::endl;
    (void) client;
    (void) serveur;
    (void) channel;
    (void) args;
    (void) client;
    return 0;
}

void fillModesMap(std::map<std::string, ModeFunction>& modes)
{
    (void) modes;
    modes["+i"] = setInvitation;
    modes["-i"] = removeInvitation;
    modes["+k"] = setKeyPass;
    modes["-k"] = removeKeyPass;
    modes["+o"] = addOperator;
    modes["-o"] = removeOperator;
    modes["+l"] = setLimitUser;
    modes["-l"] = removeLimitUser;
    modes["+t"] = setTopicProtection;
    modes["-t"] = removeTopicProtection;
}


int handleMode(const std::string& line, Client& client, Serveur& serveur) 
{
    std::vector<std::string> args = createArg(line);
    std::string response;

    
    if (args.size() < 2) 
    {
        response = client.returnPrefixe() + ERR_NEEDMOREPARAMS(args[0]) + "\r\n";
        sendResponse(client, serveur, response);
        return 1;
    } 
    std::string reason   = createReasonMode(line, 0);
    

    if (args[1][0] == '#' && args.size() > 2) {
        std::string channelStr = args[1];
        Channel* channel = serveur.getChannel(channelStr);
        if (channel->checkIfClientOperator(client.getNick()))
        {
            response = client.returnPrefixe() + ERR_CHANOPRIVSNEEDED(channelStr) + "\r\n";
            sendResponse(client, serveur, response);
            return 1;
        }
        if (args[2][0] == '+' || args[2][0] == '-')
        {
            if (findMode(&client, args, serveur, *channel) == 1)
            {
                return 1;
            }
        }
        else
        {
            response = client.returnPrefixe() +  ERR_NEEDMOREPARAMS(args[2]) + "\r\n";
            sendResponse(client, serveur, response);
            return 1;
        }
    }
    response = client.returnPrefixe() + "MODE " + reason + "\r\n";
    sendResponse(client, serveur, response);
    return (0);
}


/*

    check472 dans le code et faire en sorte de mettre les erreurs quand les mode sont invalide


    <-- Received = MODE #lol b

    --> send2 = :cricri!sbelabba@127.0.0.1 367 cricri #lol  127.0.0.1 1699882674

    --> send2 = :cricri!sbelabba@127.0.0.1 368 cricri #lol:End of channel ban list


    <-- Received = MODE #lol +it

    --> DEBUG MODE : pars 4 = ><
    --> send = :cricri!sbelabba@127.0.0.1 MODE #lol +i 
    --> send = :cricri!sbelabba@127.0.0.1 MODE #lol +t 

    <-- Received = MODE #lol +io

    --> sendReply = :cricri!sbelabba@127.0.0.1 696 cricri  cricri@localhost #lol +io  :mode parameter needed
    --> sendReply = :cricri!sbelabba@127.0.0.1 696 cricri  cricri@localhost #lol +io  :invalid number of mode parameters


    <-- Received = MODE #lol +o

    --> sendReply = :cricri!sbelabba@127.0.0.1 696 cricri  cricri@localhost #lol +o  :mode parameter needed
    --> sendReply = :cricri!sbelabba@127.0.0.1 696 cricri  cricri@localhost #lol +o  :invalid number of mode parameters


    <-- Received = MODE #lol +i

    --> send = :cricri!sbelabba@127.0.0.1 MODE #lol +i 


*/