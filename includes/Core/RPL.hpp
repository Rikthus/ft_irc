#pragma once

#include "ft_irc.hpp"
#include "Channel.hpp"

#include <ctime>
#include <sstream>

#define USERMODE_CHARLIST ""
#define CHANNELMODE_CHARLIST "oitlk"
#define NR_ARG int const &fd, const std::string& cNick

class Channel;

class Rep
{
	private:
		std::stringstream	output;
	public:
		Rep();
		void	send_to_client(std::string msg, int const &fd);
		void	sendListOfUsers(int clientSockfd, std::string nick, Channel *chan);
		void	sendToChannel(std::string msg, Channel *chan, int skipSockfd);
		
				/* ----- Replies ----- */
		void R001(NR_ARG, std::string pigeon);
		void R002(NR_ARG, const std::string& servName, const std::string &servVersion);
		void R003(NR_ARG, const std::string& date);
		void R004(NR_ARG);

		void R324(NR_ARG, const std::string& chanName, const std::string& chanModeStr, const std::string& chanModeArgs);

		void R331(NR_ARG, const std::string& chanName);
		void R332(NR_ARG, const std::string& chanName, const std::string& topic);

		void R341(NR_ARG, const std::string& otherNick, const std::string& chanName);

		void R353(NR_ARG, const std::string& chanName, const std::string& nick, char chanPrefix, char userPrefix);
		void R366(NR_ARG, const std::string& chanName);

				/* ----- Errors ----- */
		void E401(NR_ARG, const std::string& inputNick);
		void E403(NR_ARG, const std::string& chanName);

		void E412(NR_ARG);

		void E431(NR_ARG);
		void E432(NR_ARG, const std::string& badNick);
		void E433(NR_ARG, const std::string& badNick);

		void E441(NR_ARG, const std::string& chanName, const std::string& inputNick);
		void E442(NR_ARG, const std::string& chanName);
		void E443(NR_ARG, const std::string& chanName, const std::string& inputNick);

		void E451(int const &fd, const std::string &cNick);

		void E461(NR_ARG, const std::string& cmd);
		void E462(NR_ARG);
		void E464(NR_ARG);

		void E468(int const &fd, const std::string &cNick);

		void E471(NR_ARG, const std::string& chanName);
		void E472(NR_ARG, const char& 		 modeChar);
		void E473(NR_ARG, const std::string& chanName);
		void E475(NR_ARG, const std::string& chanName);
		void E476(NR_ARG);
		void E482(NR_ARG, const std::string& chanName);

		void E501(NR_ARG);

		void	clearBuffer();
};
