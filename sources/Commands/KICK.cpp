#include "../../includes/Commands/Commands.hpp"

void	KICK::execute(Server *server, clientIt &it, std::vector<std::string> args)
{
	if (args.size() < 3)
		Rep().E461(it->first, it->second.getNickname(), "KICK");
	else if (args[1].size() < 2 || args[1][0] != '#')
		Rep().E476(it->first, args[1]);
	else if (!server->findChannel(args[1]))
		Rep().E403(it->first, it->second.getNickname(), args[1]);
	else if (server->clientIsInChannel(it->first, args[1]) == false)
		Rep().E442(it->first, it->second.getNickname(), args[1]);
	else if (server->clientIsOperator(it->first, args[1]) == false)
		Rep().E482(it->first, it->second.getNickname(), args[1]);
	else if (args.size() > 3 && args[3][0] != ':')
		Rep().E442(it->first, it->second.getNickname(), args[1]);
	else
	{
		Channel	*chan = server->findChannel(args[1]);
		if (!chan->clientIsInChan(args[2]))
			Rep().E441(it->first, it->second.getNickname(), args[1], args[2]);
		else if (!server->clientIsOperator(it->first, args[1]))
			Rep().E482(it->first, it->second.getNickname(), args[1]);
		else
		{
			std::string	msg = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@irc.project.com KICK " + args[1] + " " + args[2];
			for (unsigned long i = 3; i < args.size(); i++)
				msg += ' ' + args[i];
			msg.append("\r\n");
			std::map<int, Client *>				&clientList = chan->getClientList();
			std::map<int, Client *>::iterator	itClientList;
			int									toKickSockfd = 0;
			for (itClientList = clientList.begin(); itClientList != clientList.end(); itClientList++)
			{
				if (itClientList->second->getNickname() == args[2])
				{
					toKickSockfd = itClientList->second->getFd();
					clientList.erase(itClientList);
					send(toKickSockfd, msg.c_str(), msg.size(), 0);
					break ;
				}
			}
			Rep().sendToChannel(msg, chan, toKickSockfd);
		}
	}
}

KICK::KICK(void) {}
KICK::~KICK(void) {}