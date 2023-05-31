#include "../../includes/Commands/Commands.hpp"

// SHOULD BE ABLE TO KICK MORE THAN ONE PERSON AT A TIME ?

void	KICK::execute(Server *server, clientIt &it, std::vector<std::string> args)
{
	if (args.size() != 3) //??????????????????????????????????????????????
		Rep().E461(it->first, it->second.getNickname(), "KICK");
	else if (args[1].size() < 2 || args[1][0] != '#')
		Rep().E476(it->first, args[1]);
	else if (!server->findChannel(args[1]))
		Rep().E403(it->first, it->second.getNickname(), args[1]);
	else if (server->clientIsInChannel(it->first, args[1]) == false)
		Rep().E442(it->first, it->second.getNickname(), args[1]);
	else if (server->clientIsOperator(it->first, args[1]) == false)
		Rep().E482(it->first, it->second.getNickname(), args[1]);
	else
	{
		Channel	*chan = server->findChannel(args[1]);
		if (!chan->clientIsInChan(args[2]))
			Rep().E441(it->first, it->second.getNickname(), args[1], args[2]);
		else if (!server->clientIsOperator(it->first, args[1]))
			Rep().E482(it->first, it->second.getNickname(), args[1]);
		else
		{
			std::string	addr = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@" + inet_ntoa(it->second.getAddr()) + " KICK " + args[1] + " " + args[2] + "\r\n";
			send(it->first, addr.c_str(), addr.size(), 0);
		}
	}
}

KICK::KICK(void) {}
KICK::~KICK(void) {}