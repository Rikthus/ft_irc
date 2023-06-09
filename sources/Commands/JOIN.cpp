#include "../../includes/Commands/Commands.hpp"
//*******************  ABSTRACT CLASS  ********************////

ACmd::ACmd(void) {}
ACmd::~ACmd(void) {}

//********************************  JOIN  ****************************////

std::vector<std::string>	JOIN::trimArgs(std::string args)
{
	std::vector<std::string>	channels;
	std::string	line;
	std::istringstream iss(args);

	while (std::getline(iss, line, ','))
	{
		if (!line.empty())
			channels.push_back(line);
	}
	return (channels);
}

void	JOIN::execute(Server *server, clientIt &it, std::vector<std::string> args)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;

	if (!it->second.getRegistration())
		return Rep().E451(it->first, it->second.getNickname());
	if (args.size() < 2 || args.size() > 3)
		return Rep().E461(it->first, it->second.getNickname(), "JOIN");
	channels = this->trimArgs(args[1]);
	if (args.size() == 3)
		keys = this->trimArgs(args[2]);
	if (channels.size() < keys.size())
		Rep().E476(it->first, args[2]);
	else
	{
		for (unsigned long i = 0; i < channels.size(); i++)
		{
			if (channels[i].size() < 2 || channels[i][0] != '#')
			{
				Rep().E476(it->first, it->second.getNickname());
				continue ;
			}
			if (i < keys.size())
			{
				Channel *chan = server->findChannel(channels[i]);
				if (chan)
				{
					if (chan->clientIsInChan(it->second.getNickname()))
						Rep().E443(it->first, it->second.getNickname(), channels[i], it->second.getNickname());
					else if (server->chanAuthentication(channels[i], keys[i], it->first, it->second.getNickname()))
					{
						server->joinChan(channels[i], it->first, it->second);
						std::string	msg = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@irc.project.com JOIN " + channels[i] + "\r\n";
						send(it->first, msg.c_str(), msg.size(), 0);
						Channel *chan = server->findChannel(channels[i]);
						if (chan->getTopicSet())
							Rep().R332(it->first, it->second.getNickname(), channels[i], chan->getTopic());
						Rep().sendListOfUsers(it->first, it->second.getNickname(), chan);
						Rep().R366(it->first, it->second.getNickname(), channels[i]);
						Rep().sendToChannel(msg, chan, it->first);
					}
				}
				else
				{
					server->createChan(channels[i], it->first, it->second, keys[i], true);
					Channel	*chan = server->findChannel(channels[i]);
					chan->setPass(true, keys[i]);
					std::string	msg = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@irc.project.com JOIN " + channels[i] + "\r\n";
					send(it->first, msg.c_str(), msg.size(), 0);
					(server->findChannel(channels[i]))->setOperator(true, it->second.getNickname());
					Rep().send_to_client("MODE " + channels[i] + " +k " + keys[i], it->first);
					Rep().send_to_client("MODE " + channels[i] + " +o " + it->second.getNickname(), it->first);
				}
			}
			else
			{
				Channel *chan = server->findChannel(channels[i]);
				if (chan)
				{
					if (chan->clientIsInChan(it->second.getNickname()))
						Rep().E443(it->first, it->second.getNickname(), channels[i], it->second.getNickname());
					else if (server->chanAuthentication(channels[i], NO_PWD, it->first, it->second.getNickname()))
					{
						server->joinChan(channels[i], it->first, it->second);
						std::string	msg = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@irc.project.com JOIN " + channels[i] + "\r\n";
						send(it->first, msg.c_str(), msg.size(), 0);
						Channel *chan = server->findChannel(channels[i]);
						if (chan->getTopicSet())
							Rep().R332(it->first, it->second.getNickname(), channels[i], chan->getTopic());
						Rep().sendListOfUsers(it->first, it->second.getNickname(), chan);
						Rep().R366(it->first, it->second.getNickname(), channels[i]);	
						Rep().sendToChannel(msg, chan, it->first);
					}
				}
				else
				{
					server->createChan(channels[i], it->first, it->second, NO_PWD, false);
					std::string	msg = ":" + it->second.getNickname() + "!" + it->second.getUsername() + "@irc.project.com JOIN " + channels[i] + "\r\n";
					send(it->first, msg.c_str(), msg.size(), 0);
					(server->findChannel(channels[i]))->setOperator(true, it->second.getNickname());
					Rep().send_to_client("MODE " + channels[i] + " +o " + it->second.getNickname(), it->first);
				}
			}
		}
	}
}

JOIN::JOIN(void) {}
JOIN::~JOIN(void) {}
