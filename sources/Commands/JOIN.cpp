#include "../../includes/Commands/Commands.hpp"
//*******************  ABSTRACT CLASS  ********************////

ACmd::ACmd(void) {}
ACmd::~ACmd(void) {}

//********************************  JOIN  ****************************////

void	JOIN::execute(Server *server, clientIt &it, std::vector<std::string> args)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;

	int		nbChan = 0;
	int		nbKey = 0;
	bool	possibleChan = true;
	bool	possibleKey = false;

	if (args.size() < 2)
	{
		Rep().E461(it->first, it->second.getNickname(), "JOIN");
		return ;
	}
	for (unsigned long i = 1; i < args.size(); i++)
	{
		if (possibleChan && args[i].size() > 1 && args[i][0] == '#' && keys.size() == 0)
		{
			channels.push_back(args[i]);
			nbChan++;
			possibleKey = true;
		}
		else if (args[i].size() < 2 && (!possibleKey || args[i][0] == '#'))
		{
			Rep().E476(it->first, args[i]);
			return ;
		}
		else if (possibleKey && nbKey < nbChan)
		{
			keys.push_back(args[i]);
			nbKey++;
		}
		else
		{
			Rep().E476(it->first, args[i]);
			return ;
		}
	}
	for (unsigned long i = 0; i < channels.size(); i++)
	{
		if (i < keys.size())
		{
			if (server->findChannel(channels[i]))
			{
				if (server->chanAuthentication(channels[i], keys[i], it->first, it->second.getNickname()))
				{
					server->joinChan(channels[i], it->first, it->second);
					std::string	confirmation;
					std::string	addr = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@" + inet_ntoa(it->second.getAddr()) + " JOIN " + channels[i] + "\r\n";
					send(it->first, addr.c_str(), addr.size(), 0);
					Channel *chan = server->findChannel(channels[i]);
					if (chan->getTopicSet())
						Rep().R332(it->first, it->second.getNickname(), channels[i], chan->getTopic());
					confirmation = ":irc.project.com 353 " + it->second.getNickname() + " " + channels[i] + " :" + it->second.getNickname() + "\r\n";
					Rep().send_to_channel(confirmation, chan);
					Rep().R366(it->first, it->second.getNickname(), channels[i]);
				}
			}
			else
			{
				server->createChan(channels[i], it->first, it->second, keys[i], true);
				Channel	*chan = server->findChannel(channels[i]);
				chan->setPass(true, keys[i]);
				Rep().send_to_client("MODE " + channels[i] + " +k " + keys[i], it->first);
				std::string	addr = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@" + inet_ntoa(it->second.getAddr()) + " JOIN " + channels[i] + "\r\n";
				send(it->first, addr.c_str(), addr.size(), 0);
				(server->findChannel(channels[i]))->setOperator(true, it->second.getNickname());
				Rep().send_to_client("MODE " + channels[i] + " +o " + it->second.getNickname(), it->first);
			}
		}
		else
		{
			if (server->findChannel(channels[i]))
			{
				if (server->chanAuthentication(channels[i], NO_PWD, it->first, it->second.getNickname()))
				{
					server->joinChan(channels[i], it->first, it->second);
					std::string	confirmation;
					std::string	addr = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@" + inet_ntoa(it->second.getAddr()) + " JOIN " + channels[i] + "\r\n";
					send(it->first, addr.c_str(), addr.size(), 0);
					Channel *chan = server->findChannel(channels[i]);
					if (chan->getTopicSet())
						Rep().R332(it->first, it->second.getNickname(), channels[i], chan->getTopic());
					confirmation = ":irc.project.com 353 " + it->second.getNickname() + " " + channels[i] + " :" + it->second.getNickname() + "\r\n";
					Rep().send_to_channel(confirmation, chan);
					Rep().R366(it->first, it->second.getNickname(), channels[i]);	
				}
			}
			else
			{
				server->createChan(channels[i], it->first, it->second, NO_PWD, false);
				std::string	addr = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@" + inet_ntoa(it->second.getAddr()) + " JOIN " + channels[i] + "\r\n";
				send(it->first, addr.c_str(), addr.size(), 0);
				(server->findChannel(channels[i]))->setOperator(true, it->second.getNickname());
				Rep().send_to_client("MODE " + channels[i] + " +o " + it->second.getNickname(), it->first);
			}
		}
	}
}

JOIN::JOIN(void) {}

JOIN::~JOIN(void) {}
