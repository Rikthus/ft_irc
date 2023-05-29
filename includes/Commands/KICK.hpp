#pragma once

# include "JOIN.hpp"

class KICK: public ACmd
{
	public:
		void execute(Server *server, int clientSockfd, Client &clientData, std::vector<std::string> args);

		KICK(void);
		~KICK(void);
};