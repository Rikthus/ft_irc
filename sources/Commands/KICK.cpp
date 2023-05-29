#include "../../includes/Commands/KICK.hpp"

void	KICK::execute(Server *server, int clientSockfd, Client &clientData, std::vector<std::string> args)
{
	if (args.size() < 3 || args[1].size() < 2 || args[1][0] != '#' || args[2][0] == '#')
		std::cout << "BAD FORMAT" << std::endl;
	else
	{
		if (!server->findChannel(args[1]))
			std::cout << "CHANNEL DOES NOT EXISTS" << std::endl;
		else if ()
	}
}

KICK::KICK(void) {}
KICK::~KICK(void) {}