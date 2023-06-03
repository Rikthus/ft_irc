#include "../../includes/Commands/Commands.hpp"

bool	INVITE::isInServer(Server *server, std::string nick)
{
	std::map<int, Client>			clientList = server->getClientList();
	std::map<int, Client>::iterator	it;

	for (it = clientList.begin(); it != clientList.end(); it++)
	{
		if (it->second.getNickname() == nick)
			return (true);
	}
	return (false);
}

void	INVITE::execute(Server *server, clientIt &it, std::vector<std::string> args)
{
	if (!it->second.getRegistration())
		return Rep().E451(it->first, it->second.getNickname());
	if (args.size() != 3)
		return Rep().E461(it->first, it->second.getNickname(), "INVITE");
	else if (args[2].size() < 2 || args[2][0] != '#')
		return Rep().E476(it->first, args[2]);
	else if (!server->findChannel(args[2]))
		return Rep().E403(it->first, it->second.getNickname(), args[2]);
	else if (server->clientIsInChannel(it->first, args[2]) == false)
		return Rep().E442(it->first, it->second.getNickname(), args[2]);
	else if (this->isInServer(server, args[1]) == false)
		return Rep().E401(it->first, it->second.getNickname(), args[2]);
	else
	{
		Channel	*chan = server->findChannel(args[2]);
		if (chan->clientIsInChan(args[1]))
			Rep().E443(it->first, it->second.getNickname(), args[2], args[1]);
		else if (server->chanModeIsSet(INVITE_ONLY, args[2]) && !server->clientIsOperator(it->first, args[2]))
			Rep().E482(it->first, it->second.getNickname(), args[2]);
		else
		{
			int invitedSockfd = server->findNickSockfd(args[1]);
			chan->addInvitation(invitedSockfd);
			std::string msg = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@irc.project.com INVITE " + args[1] + " :" + args[2] + "\n\r";
			send(invitedSockfd, msg.c_str(), msg.size(), 0);
			Rep().R341(it->first, it->second.getNickname(), args[1], args[2]);
			Rep().sendToChannel(msg, chan, it->first);
		}
	}
}

INVITE::INVITE(void) {}
INVITE::~INVITE(void) {}