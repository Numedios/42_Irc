#ifndef RESPONSES_HPP
# define RESPONSES_HPP


///////////////////////////////////////////////////////////////////////////
// CUSTOM MESSAGES

# define PING(token) ("PING :" + token) // can't touch this
# define PONG(token) ("PONG " + token) // can't touch this
# define QUIT_REASON(nick, user, host, msg) (nick + "!" + user + "@" + host  + " QUIT :" + msg)
# define QUIT(nick, user, host) (nick + "!" + user + "@" + host  + " QUIT")
# define NICK(old_nick, nick) (":" + old_nick + " NICK " + nick) // pb decla string and int
# define NICK_INFORM(old_nick, user, host, nick) (":" + old_nick + "!" + user + "@" + host + " NICK " + nick)
# define JOIN(nick, user, host, chan) (":" + nick + "!" + user + "@" + host  + " JOIN " + chan)
# define PRIVMSG(nick, user, host, dest, str) (":" + nick + "!" + user + "@" + host + " PRIVMSG " + dest + " :" + str) //dest cest le nom du channel
# define NOTICE(nick, user, host, chan, str) (":" + nick + "!" + user + "@" + host + " NOTICE " + chan + " :" + str)
# define PART(nick, user, host, chan) (":" + nick + "!" + user + "@" + host + " PART " + chan)
# define PART_REASON(nick, user, host, chan, reason) (":" + nick + "!" + user + "@" + host + " PART " + chan + " :" + reason)
# define KILL(nick, target) (":" + nick + " KILL " + target)
# define KILL_REASON(nick, target, reason) (":" + nick + " KILL " + target + " :" + reason)
# define BANNED(nick, user, host, channel, target) (":" + nick + "!" + user + "@" + host + " MODE " + channel + " +b " + target)
# define KICK(chan, kicked, reason) ("KICK " + chan + " " + kicked + " :" + reason)
# define INVITE(nick, user, host, nickinvite, channel)  (":" + nick + "!" + user + "@" + host + " INVITE " + nickinvite + " " + channel)
# define TOPIC(chan, topic) ("TOPIC " + chan + " :" + topic)
# define MYJOIN(nickname, channame) (":" + nickname + " " + "JOIN" + " " + channame)

///////////////////////////////////////////////////////////////////////////
// REPLIES

# define RPL_WELCOME(nick, user, host) (std::string("001 "+ nick + " Welcome to the Internet Relay Network ") + nick + "!" + user + "@" + host)
# define RPL_YOURHOST(server_name, version) (std::string("002:Your host is ") + server_name + ", running version" + version)
# define RPL_CREATED(date) ("003:This server was created " + std::string(date))
# define RPL_MYINFO(servername, version, usermodes, channelmodes) (std::string("004:") + servername + " " + version + " " + usermodes + " " + channelmodes)

# define RPL_UMODEIS(nick, usermode) "221 " + nick +  " " +  usermode
# define RPL_AWAY(nick, message) "301 " + nick + " :" + message
# define RPL_UNAWAY() "305:You are no longer marked as being away"
# define RPL_NOWAWAY() "306:You have been marked as being away"
# define RPL_ENDOFWHO(channel) ("315 " + channel + " :End of WHO list")
# define RPL_LIST(channel, visible, topic) "322 " + channel + " " + visible + " :" + topic
# define RPL_LISTEND() "323:End of LIST"
# define RPL_CHANNELMODEIS(channel, mode, modeParams) "324 " + channel + " " + mode + " " + modeParams
# define RPL_CREATIONTIME(client, channel, creationtime) "329 " + client + " " + channel + " " + creationtime
# define RPL_NOTOPIC(channel) "331" + channel + " :No topic is set"
# define RPL_TOPIC(channel, topic, client) "332 " + client +  " " + channel + " :" + topic // join
# define RPL_TOPICWHOTIME(client, channel, whoset, setat) "333 " + client + " " + channel + " " + whoset + " " + setat
# define RPL_INVITING(channel, nick) "341 " + channel + " " + nick
# define RPL_VERSION(version, debuglevel, server) "351 " + version + "." + debuglevel + " " + server
# define RPL_WHOREPLY(channel, user, host, server, nick, status, hopcount, realname) ("352" + channel + " " + user + " " + host + " " + server + " " + nick + " " + status + " " + ":" + hopcount + " " + realname);
# define RPL_NAMREPLY(channel, users, ops) "353 " + users + " = " + channel + " :" + ops // join
# define RPL_ENDOFNAMES(channel, users) "366 " + users + " " + channel + " :End of /NAMES list" // join
# define RPL_BANLIST(client, channel, nickmask, who, timestamp) ("367 " + client + " " + channel + " " + nickmask + " " + who + " " + timestamp) // nickmask = nick!user@host ; 
# define RPL_ENDOFBANLIST(client, channel) "368 " + client + " " + channel + ":End of channel ban list"
# define RPL_MOTDSTART(server) "375:- " + server + " Message of the day - "
# define RPL_MOTD(text) "372:- " + text
# define RPL_ENDOFMOTD() "376:End of MOTD command"
# define RPL_YOUREOPER() "381:You are now an IRC operator"
# define RPL_REHASHING(configfile) "382 " + configfile + " :Rehashing"

# define ERR_NOSUCHNICK(nickname) "401 " + nickname + " :No such nick/channel"
# define ERR_NOSUCHSERVER(servername) "402 " + servername + " :No such server"
# define ERR_NOSUCHCHANNEL(channelname) "403 " + channelname + " :No such channel"
# define ERR_CANNOTSENDTOCHAN(channelname) "404 " + channelname + " :Cannot send to channel"
# define ERR_TOOMANYCHANNELS(channelname) "405 " + channelname + " :You have joined too many channels"
# define ERR_TOOMANYTARGETS(target) "407 " + target + " :Duplicate recipients. No message delivered"
# define ERR_NOORIGIN() "409:No origin specified"
# define ERR_NORECIPIENT(command) (std::string ("411:No recipient given (") + command + ")")
# define ERR_NOTEXTTOSEND() (std::string("412:No text to send"))
# define ERR_NOTOPLEVEL(mask) "413 " + mask + " :No toplevel domain specified"
# define ERR_WILDTOPLEVEL(mask) "414 " + mask + " :Wildcard in toplevel domain"
# define ERR_NOMOTD() "422:MOTD File is missing"
# define ERR_NONICKNAMEGIVEN() (std::string ("431:No nickname given"))
# define ERR_ERRONEUSNICKNAME(nickname) "432 " + nickname + " :Erroneous nickname"
# define ERR_NICKNAMEINUSE(nickname) "433 " + nickname + " :Nickname is already in use"
# define ERR_NICKCOLLISION(nickname, username, host) "436" + nickname + " :Nickname collision KILL from " + username + "@" + host
# define ERR_UNAVAILRESOURCE(nickchannel) "437 " + nickchannel + " :Nick/channel is temporarily unavailable"
# define ERR_USERNOTINCHANNEL(nickname, channel) "441 " + nickname + " " + channel + " :not on this channel"
# define ERR_NOTONCHANNEL(channel) "442 " + channel + " :You're not on that channel"
# define ERR_USERONCHANNEL(username, channel) "443 " + username + " " + channel + " :is already on channel"
# define ERR_NOTREGISTERED(client) "451 " + client + ": You have not registered"
# define ERR_NEEDMOREPARAMS(command) (std::string ("461 ") + command + " :Not enough parameters")
# define ERR_ALREADYREGISTRED() "462:Unauthorized command (already registered)"
# define ERR_PASSWDMISMATCH() "464:Password incorrect"
# define ERR_CHANNELISFULL(name, channel) "471 " + name + " " + channel + " :Cannot join channel (+l)"
# define ERR_UNKNOWNMODE(mode, channel) "472 " + mode + " :is unknown mode char to me for " + channel
# define ERR_INVITEONLYCHAN(client, channel) "473 " + client +  " " + channel + " :Cannot join channel (+i)"
# define ERR_BANNEDFROMCHAN(channel) "474 " + channel + " :Cannot join channel (+b)"
# define ERR_BADCHANNELKEY(name, channel) "475 " + name + " " + channel + " :Cannot join channel (+k)"
# define ERR_BADCHANMASK(channel) "476 " + channel + ": Bad Channel Mask"
# define ERR_NOCHANMODES(channel) "477 " + channel + " :Channel doesn't support modes"
# define ERR_NOPRIVILEGES() "481:Permission Denied- You're not an IRC operator"
# define ERR_CHANOPRIVSNEEDED(channel) "482 " + channel + " :You're not channel operator"
# define ERR_CANTKILLSERVER() "483:You can't kill a server!"
# define ERR_RESTRICTED() "484:Your connection is restricted!"
# define ERR_NOOPERHOST() "491:No O-lines for your host"
# define ERR_UMODEUNKNOWNFLAG() "501:Unknown MODE flag"
# define ERR_USERSDONTMATCH() "502:Cannot change mode for other users"
# define ERR_INVALIDMODEPARAM(client, target, mode, parameter, description) "696 " + client + " " + target + " " + mode + " " + parameter + " :" + description

#endif