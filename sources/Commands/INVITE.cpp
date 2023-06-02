#include "../../includes/Commands/Commands.hpp"

void	INVITE::execute(Server *server, clientIt &it, std::vector<std::string> args)
{
	if (args.size() != 3)
		Rep().E461(it->first, it->second.getNickname(), "INVITE");
	else if (args[2].size() < 2 || args[2][0] != '#')
		Rep().E476(it->first, args[2]);
	else if (!server->findChannel(args[2]))
		Rep().E403(it->first, it->second.getNickname(), args[2]);
	else if (server->clientIsInChannel(it->first, args[2]) == false)
		Rep().E442(it->first, it->second.getNickname(), args[2]);
	else
	{
		Channel	*chan = server->findChannel(args[2]);
		if (chan->clientIsInChan(args[2]))
			Rep().E443(it->first, it->second.getNickname(), args[2], args[1]);
		else if (server->chanModeIsSet(INVITE_ONLY, args[2]) && !server->clientIsOperator(it->first, args[2]))
			Rep().E482(it->first, it->second.getNickname(), args[2]);
		else
		{
			int invitedSockfd = server->findNickSockfd(args[1]);
			chan->addInvitation(invitedSockfd);
			std::string addr = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@irc.project.com INVITE " + args[1] + " :" + args[2] + "\n\r";
			send(invitedSockfd, addr.c_str(), addr.size(), 0);
			Rep().R341(it->first, it->second.getNickname(), args[1], args[2]);
		}
	}
}

INVITE::INVITE(void) {}
INVITE::~INVITE(void) {}