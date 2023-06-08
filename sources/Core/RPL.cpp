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

void Rep::sendListOfUsers(int clientSockfd, std::string nick, Channel *chan)
{
	std::map<int, Client *>				&clientList = chan->getClientList();
	std::map<int, Client *>::iterator	it;
	std::string							msg;
	for (it = clientList.begin(); it != clientList.end(); it++)
	{
		msg = ":irc.project.com 353 " + nick + " " + chan->getName() + " :" + it->second->getNickname() + "\r\n";
		send(clientSockfd, msg.c_str(), msg.size(), 0);
	}
}

void Rep::sendToChannel(std::string msg, Channel *chan, int skipSockfd)
{
	std::map<int, Client *>				&clientList = chan->getClientList();
	std::map<int, Client *>::iterator	it;

	for (it = clientList.begin(); it != clientList.end(); it++)
	{
		if (it->first != skipSockfd)
			send(it->first, msg.c_str(), msg.size(), 0);
	}
}

/**
 * @brief RPL_WELCOME
 * @param fd 
 * @param cNick 
 */
void Rep::R001(int const &fd, const std::string &cNick, std::string pigeon)
{
	output << "001 " << cNick << " :Welcome " << cNick << " to the Internet Chat Relay!\r\n";
	output << pigeon;
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

void Rep::E421(int const &fd, const std::string &cNick, const std::string& cmd)
{
	output << "421 " << cNick << " " << cmd << " :Unknown command";
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
	std::cout << "output:" << output.str() << std::endl;
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

void	Rep::clearBuffer()
{
	output.str("");
}