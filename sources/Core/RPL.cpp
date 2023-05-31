#include "../../includes/Core/RPL.hpp"

int	send_to_user(std::string msg, int sockfd)
{
	return (send(sockfd, msg.c_str(), msg.size(), 0));
}

Rep::Rep() {}

void Rep::send_to_client(std::string msg, int const &fd) {
	msg = ":" + std::string("irc.project.com") + " " + msg +"\r\n";
	if (send_to_user(msg, fd) == -1)
		throw(std::runtime_error("Error: send function failed"));

	//cout << ANSI::gray << "{send} => " << ANSI::purple << msg << endl;
}

void Rep::send_to_channel(std::string msg, Channel *chan)
{
	std::map<int, Client *>::iterator	it;

	for (it = chan->mClientList.begin(); it != chan->mClientList.end(); it++)
		send(it->first, msg.c_str(), msg.size(), 0);
}

/**
 * @brief RPL_WELCOME
 * @param fd 
 * @param cNick 
 */
void Rep::R001(int const &fd, const std::string &cNick)
{
	output << "001 " << cNick << " :Welcome " << cNick << " to the Internet Chat Relay!";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_YOURHOST
 * @param fd 
 * @param cNick 
 * @param servName 
 * @param servVersion 
 */
void Rep::R002(int const &fd, const std::string &cNick, const std::string& servName, const std::string &servVersion)
{
	output << "002 " << cNick << " :Your host is " << servName << ", running version " << servVersion;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_CREATED
 * @param fd 
 * @param cNick 
 * @param date 
 */
void Rep::R003(int const &fd, const std::string &cNick, const std::string& date)
{
	output << "003 " << cNick << " :This server was created " << date;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_MYINFO
 * @param fd 
 * @param cNick 
 */
void Rep::R004(int const &fd, const std::string &cNick)
{
	/* User modes https://www.rfc-editor.org/rfc/rfc2812#section-3.1.5 */
	/* Channel modes https://www.rfc-editor.org/rfc/rfc2811#section-4 */
	output << "004 " << cNick << " :`the best irc project wallah` 0.42 " << CHANNELMODE_CHARLIST << " " << USERMODE_CHARLIST;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_BOUNCE
 * @param fd 
 * @param cNick 
 * @param infostr 
 */
void Rep::R211(int const &fd, const std::string &cNick, const std::string& infostr)
{
	output << "211 " << cNick << " " << infostr;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_STATSLINKINFO
 * @param fd 
 * @param cNick 
 * @param infostr 
 */
void Rep::R212(int const &fd, const std::string &cNick, const std::string& infostr)
{
	output << "212 " << cNick << " " << infostr << " 0 0";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_STATSCOMMANDS
 * @param fd 
 * @param cNick 
 * @param infostr 
 */
void Rep::R219(int const &fd, const std::string &cNick, const std::string& letters)
{
	output << "219 " << cNick << " " << letters << " :End of STATS report";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_UMODEIS
 * @param fd 
 * @param cNick 
 * @param userModes 
 */
void	Rep::R221(int const &fd, const std::string &cNick, const std::string& userModes)
{
	output << "221 " << cNick << " " << userModes;
	send_to_client(output.str(), fd);
	clearBuffer();
}

void Rep::R242(int const &fd, const std::string &cNick, const std::string& infostr)
{
	output << "242 " << cNick << " :" << infostr;
	send_to_client(output.str(), fd);
	clearBuffer();
}

void Rep::R243(int const &fd, const std::string &cNick, const std::string& infostr)
{
	output << "243 " << cNick << " " << infostr;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_LUSERCLIENT
 * @param fd 
 * @param cNick 
 * @param infostr 
 */
void Rep::R251(int const &fd, const std::string &cNick, const std::string& infostr)
{
	output << "251 " << cNick << " " << infostr;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_LUSEROP
 * @param fd 
 * @param cNick 
 * @param ops 
 */
void Rep::R252(int const &fd, const std::string &cNick, int ops)
{
	output << "252 " << cNick << " " << ops << " :operator(s) online";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_LUSERUNKNOWN
 * @param fd 
 * @param cNick 
 * @param unknownConnnections 
 */
void Rep::R253(int const &fd, const std::string &cNick, int unknownConnnections)
{
	output << "253 " << cNick << " " << unknownConnnections << " :unknown connection(s)";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_LUSERCHANNELS
 * @param fd 
 * @param cNick 
 * @param channels 
 */
void Rep::R254(int const &fd, const std::string &cNick, int channels)
{
	output << "254 " << cNick << " " << channels << " :channel(s) currently open";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_LUSERME
 * @param fd 
 * @param cNick 
 * @param infostr 
 */
void Rep::R255(int const &fd, const std::string &cNick, const std::string& infostr)
{
	output << "255 " << cNick << " " << infostr;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_WHATCHSTAT
 * @param fd 
 * @param cNick 
 * @param users 
 */
void Rep::R266(int const &fd, const std::string &cNick, int users)
{
	output << "266 " << cNick << " " << users << " " << 1024 <<" :Current global users " << users << ", max " << 1024;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_AWAY
 * @param fd
 * @param cNick
 * @param queryNick
 * @param awayMessage
 */
void Rep::R301(int const &fd, const std::string &cNick, const std::string& queryNick, const std::string& awayMessage)
{
	output << "301 " << cNick << " " << queryNick << " :" << awayMessage;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_WHOISUSER
 * @param fd 
 * @param cNick 
 * @param queryNick 
 * @param queryName 
 * @param queryRealName 
 */
void Rep::R311(int const &fd, const std::string &cNick, const std::string& queryNick, const std::string& queryName, const std::string& queryRealName)
{
	output << "311 " << cNick << " " << queryNick << " " << queryName << " * * :" << queryRealName;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_WHOISOPERATOR
 * @param fd 
 * @param cNick 
 * @param queryNick 
 */
void Rep::R313(int const &fd, const std::string &cNick, const std::string& queryNick)
{
	output << "313 " << cNick << " " << queryNick << " :Is an IRC operator";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_ENDOFWHO
 * @param fd 
 * @param cNick 
 * @param name 
 */
void Rep::R315(int const &fd, const std::string &cNick, const std::string& name)
{
	output << "315 " << cNick << " " << name << " :End of Who query";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_ENDOFWHOIS
 * @param fd 
 * @param cNick 
 * @param queryList 
 */
void Rep::R318(int const &fd, const std::string &cNick, const std::string& queryList)
{
	output << "318 " << cNick << " " << queryList << " :End of /WHOIS list";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_WHOISCHANNELS
 * @param fd 
 * @param cNick 
 * @param queryNick 
 * @param prefix 
 * @param chanName 
 */
void Rep::R319(int const &fd, const std::string &cNick, const std::string& queryNick, char prefix, const std::string& chanName)
{
	output << "319 " << cNick << " " << queryNick << " :";
	if (prefix != 'u')
	 	output << prefix;
	output << chanName;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_LISTSTART
 * @param fd 
 * @param cNick 
 */
void Rep::R321(int const &fd, const std::string &cNick)
{
	output << "321 " << cNick << " Channel :Users Name";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_LIST
 * @param fd 
 * @param cNick 
 * @param queryNick 
 * @param serverName 
 * @param serverInfo 
 */
void	Rep::R322(int const &fd, const std::string &cNick, unsigned int nuser, const std::string& topic, const std::string& chanName)
{
	output << "322 " << cNick << " " << chanName << " " << nuser << " :" << topic;
	send_to_client(output.str(), fd);
	clearBuffer();
}
/*
:*.freenode.net 322 ben #shavik-usb 1 :[+nt] 
:*.freenode.net 323 ben :End of channel list.

*/



/**
 * @brief RPL_LISTEND
 * @param fd 
 * @param cNick 
 */
void	Rep::R323(int const &fd, const std::string &cNick)
{
	output << "323 " << cNick << " :End of LIST";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_CHANNELMODEIS
 * @param fd 
 * @param cNick 
 * @param chanName 
 * @param chanModeStr 
 * @param chanModeArgs 
 */
void	Rep::R324(int const &fd, const std::string &cNick, const std::string& chanName, const std::string& chanModeStr, const std::string& chanModeArgs)
{
	output << "324 " << cNick << " " << chanName << " " << chanModeStr << " " << chanModeArgs;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_CREATIONTIME
 * @param fd
 * @param cNick
 * @param chanName
 * @param creationTime
 */
void Rep::R329(int const &fd, const std::string &cNick, const std::string &chanName, const std::string &creationTime)
{
	output << "329 " << cNick << " " << chanName << " " << creationTime;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_NOTOPIC
 * @param fd 
 * @param cNick 
 * @param chanName 
 */
void Rep::R331(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "331 " << cNick << " " << chanName << " :No topic set";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_TOPIC
 * @param fd 
 * @param cNick 
 * @param chanName 
 * @param topic 
 */
void Rep::R332(int const &fd, const std::string &cNick, const std::string& chanName, const std::string& topic)
{
	output << "332 " << cNick << " " << chanName << " " << topic;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief 
 * @param fd 
 * @param cNick 
 * @param chanName 
 * @param setterNick 
 * @param timestamp 
 */
void Rep::R333(int const &fd, const std::string &cNick, const std::string& chanName, const std::string& setterNick, time_t timestamp)
{
	// msg = ":" + _client[fd].get_nick() + "!" + _client[fd].get_username() + "@" + std::string(SERVER_NAME) + " " + msg + "\r\n";
	output << "333 " << cNick << " " << chanName << " " << setterNick << " :" << timestamp;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief RPL_INVITING
 * @param fd 
 * @param cNick 
 * @param otherNick 
 * @param chanName 
 */
void Rep::R341(int const &fd, const std::string &cNick, const std::string& otherNick, const std::string& chanName)
{
	output << "341 " << cNick << " " << otherNick << " " << chanName;
	send_to_client(output.str(), fd);
	clearBuffer();
}


void Rep::R352(int const &fd, const std::string &cNick, const std::string& message)
{
	output << "352 " << cNick << " " << message;
	send_to_client(output.str(), fd);
	clearBuffer();
}

void Rep::R353(int const &fd, const std::string &cNick, const std::string& chanName, const std::string& nick, char chanPrefix, char userPrefix)
{
	if (userPrefix == 'u')
		output << "353 " << cNick << " " << chanPrefix << " " << chanName << " :" << nick;
	else
		output << "353 " << cNick << " " << chanPrefix << " " << chanName << " :" << userPrefix << nick;
	send_to_client(output.str(), fd);
	clearBuffer();
}

void Rep::R366(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "366 " << cNick << " " << chanName << " :End of NAMES list";
	send_to_client(output.str(), fd);
	clearBuffer();
}

void Rep::R367(int const &fd, const std::string &cNick, const std::string& chanName, const std::string& bannedUser)
{
	output << "367 " << cNick << " " << chanName << " " << bannedUser;
	send_to_client(output.str(), fd);
	clearBuffer();
}

void Rep::R368(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "368 " << cNick << " " << chanName << " :End of channel ban list";
	send_to_client(output.str(), fd);
	clearBuffer();
}

void	Rep::R372(int const &fd, const std::string &cNick, const std::string& Motd)
{
	output << "372 " << cNick << " :" << Motd;
	send_to_client(output.str(), fd);
	clearBuffer();
}

void	Rep::R375(int const &fd, const std::string &cNick, const std::string& serverName)
{
	output << "375 " << cNick << " :- " << serverName << " Message of the day - ";
	send_to_client(output.str(), fd);
	clearBuffer();
}

void	Rep::R376(int const &fd, const std::string &cNick)
{
	output << "376 " << cNick << " :End of /MOTD command.";
	send_to_client(output.str(), fd);
	clearBuffer();
}

void	Rep::R381(int const &fd, const std::string &cNick)
{
	output << "381 " << cNick << " " << " :You are now an IRC operator";
	send_to_client(output.str(), fd);
	clearBuffer();
}

void Rep::R391(int const &fd, const std::string &cNick, const std::string& servName)
{
	char date_string[128];
	time_t curr_time;
	tm *curr_tm;
	time(&curr_time);
	curr_tm = localtime(&curr_time);

	strftime(date_string, 50, "%c", curr_tm);

	output << "391 " << cNick << " " << servName << " :" << date_string;
	send_to_client(output.str(), fd);
	clearBuffer();
}

/* Errors */

/**
 * @brief No such nick/channel
 * 
 * @param fd 
 * @param cNick 
 * @param inputNick 
 */
void Rep::E401(int const &fd, const std::string &cNick, const std::string& inputNick)
{
	output << "401 " << cNick << " " << inputNick << " :No such nick/channel";
	send_to_client(output.str(), fd);
	clearBuffer();
}

void Rep::E402(int const &fd, const std::string &cNick, const std::string& servName)
{
	output << "402 " << cNick << " " << servName << " :No such server";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief No such channel
 * 
 * @param fd 
 * @param cNick 
 * @param chanName 
 */
void Rep::E403(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "403 " << cNick << " " << chanName << " :No such channel";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief :Cannot send to channel
 * 
 * @param fd 
 * @param cNick 
 * @param chanName 
 */
void Rep::E404(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "404 " << cNick << " " << chanName << " :You cannot send messages to this channel";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief You have joined too many channels
 * 
 * @param fd 
 * @param cNick 
 * @param chanName 
 */
void Rep::E405(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "405 " << cNick << " " << chanName <<  " :You have joined too many channels";	
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief No origin specified
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E409(int const &fd, const std::string &cNick)
{
	output << "409 " << cNick << " :No origin specified";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief No recipient given
 * 
 * @param fd 
 * @param cNick 
 * @param cmd 
 */
void Rep::E411(int const &fd, const std::string &cNick, const std::string& cmd)
{
	output << "411 " << cNick << " :No recipient given (" << cmd << ")";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief No text to send
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E412(int const &fd, const std::string &cNick)
{
	output << "412 " << cNick << " :No text to send";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief ERR_TOOMANYMATCHES
 * 
 * @param fd
 * @param cNick
 * @param cmd
 */
void Rep::E416(int const &fd, const std::string &cNick, const std::string& cmd)
{
	output << "416 " << cNick << " " << cmd << " :Too many matches";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief Unknown command
 * 
 * @param fd 
 * @param cNick 
 * @param cmd 
 */
void Rep::E421(int const &fd, const std::string &cNick, const std::string& cmd)
{
	output << "421 " << cNick << " " << cmd << " :Unknown command";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief No MOTD in config File
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E422(int const &fd, const std::string &cNick)
{
	output << "422 " << cNick << " :No MOTD in config File";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief No nickname given
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E431(int const &fd, const std::string &cNick)
{
	output << "431 " << cNick << " :No nickname given";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief Erronous nickname
 * 
 * @param fd 
 * @param cNick 
 * @param badNick 
 */
void Rep::E432(int const &fd, const std::string &cNick, const std::string& badNick)
{
	output << "432 " << cNick << " " << badNick << " :Erronous nickname";
	send_to_client(output.str(), fd);
	clearBuffer();
}

void Rep::E433(int const &fd, const std::string &cNick, const std::string& badNick)
{
	output << "433 " << cNick << " " << badNick << " :Nickname is already in user";
	send_to_client(output.str(), fd);
	clearBuffer();
}

void Rep::E441(int const &fd, const std::string &cNick, const std::string& chanName, const std::string& inputNick)
{
	output << "441 " << cNick << " " << inputNick << " " << chanName << " :They aren't on that channel";
	send_to_client(output.str(), fd);
	clearBuffer();
}

void Rep::E442(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "442 " << cNick << " " << chanName << " :You're not on that channel";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief Is already on channel
 * 
 * @param fd 
 * @param cNick 
 * @param chanName 
 * @param inputNick 
 */
void Rep::E443(int const &fd, const std::string &cNick, const std::string& chanName, const std::string& inputNick)
{
	output << "443 " << cNick << " " << inputNick << " " << chanName << " :Is already on channel";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief :You have not registered
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E451(int const &fd, const std::string &cNick)
{
	output << "451 " << cNick << " :You have not registered";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief Not enough parameters
 * 
 * @param fd 
 * @param cNick 
 * @param cmd 
 */
void Rep::E461(int const &fd, const std::string &cNick,const std::string& cmd)
{
	output << "461 " << cNick << " " << cmd << " :Not enough parameters";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief :Unauthorized command (already registered)
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E462(int const &fd, const std::string &cNick)
{
	output << "462 " << cNick << " " << " :Unauthorized command (already registered)";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief :Password incorrect
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E464(int const &fd, const std::string &cNick)
{
	output << "464 " << cNick << " :Password incorrect";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief  USER :Your username is not valid
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E468(int const &fd, const std::string &cNick)
{
	output << "468 " << cNick << " USER :Your username is not valid";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief You are banned from this server
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E465(int const &fd, const std::string &cNick)
{
	output << "465 " << cNick << " :You are banned from this server";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief Cannot join channel
 * 
 * @param fd 
 * @param cNick 
 * @param chanName 
 */
void Rep::E471(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "471 " << cNick << " " << chanName << " :Cannot join channel (<<l)";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief is unknown mode char to me
 * 
 * @param fd 
 * @param cNick 
 * @param modeChar 
 */
void Rep::E472(int const &fd, const std::string &cNick, const char& modeChar)
{
	output << "472 " << cNick << " " << modeChar << " :is unknown mode char to me";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief :Cannot join channel (+i)
 * 
 * @param fd 
 * @param cNick 
 * @param chanName 
 */
void Rep::E473(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "473 " << cNick << " " << chanName << " :Cannot join channel (+i)";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief Cannot join channel (+b)
 * 
 * @param fd 
 * @param cNick 
 * @param chanName 
 */
void Rep::E474(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "474 " << cNick << " " << chanName << " :Cannot join channel (+b)";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief Cannot join channel (+k)
 * 
 * @param fd 
 * @param cNick 
 * @param chanName 
 */
void Rep::E475(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "475 " << cNick << " " << chanName << " :Cannot join channel (+k)";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief Bad Channel Mask
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E476(int const &fd, const std::string &chanName)
{
	output << "476 " << chanName << " :Bad Channel Mask";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief You're not an IRC operator
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E481(int const &fd, const std::string &cNick)
{
	output << "481 " << cNick << " :Permission Denied- You're not an IRC operator";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief You're not channel operator
 * 
 * @param fd 
 * @param cNick 
 * @param chanName 
 */
void Rep::E482(int const &fd, const std::string &cNick, const std::string& chanName)
{
	output << "482 " << cNick << " " << chanName << " :You're not channel operator";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief There is no Operator allowed
 * 
 * @param fd 
 * @param cNick 
 * @param chanName 
 */
void Rep::E491(int const &fd, const std::string &cNick)
{
	output << "491 " << cNick  << " :No O-lines for your host";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief Unknown MODE flag
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E501(int const &fd, const std::string &cNick)
{
	output << "501 " << cNick << " :Unknown MODE flag";
	send_to_client(output.str(), fd);
	clearBuffer();
}

/**
 * @brief Can't view or change mode for other users
 * 
 * @param fd 
 * @param cNick 
 */
void Rep::E502(int const &fd, const std::string &cNick)
{
	output << "502 " << cNick << " :Can't view or change mode for other users";
	send_to_client(output.str(), fd);
	clearBuffer();
}

void	Rep::clearBuffer()
{
	output.str("");
}