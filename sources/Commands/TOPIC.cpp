#include "../../includes/Commands/Commands.hpp"

void	TOPIC::execute(Server *server, clientIt &it, std::vector<std::string> args)
{
	if (args.size() < 2)
		Rep().E461(it->first, it->second.getNickname(), "TOPIC");
	else if (args[1].size() < 2 || args[1][0] != '#')
		Rep().E476(it->first, args[1]);
	else if (!server->findChannel(args[1]))
		Rep().E403(it->first, it->second.getNickname(), args[1]);
	else if (server->clientIsInChannel(it->first, args[1]) == false)
		Rep().E442(it->first, it->second.getNickname(), args[1]);
	Channel	*chan = server->findChannel(args[1]);
	if (args.size() == 2)
	{
		if (chan->getTopicSet())
			chan->rplTopic();
		else
			Rep().R331(it->first, it->second.getNickname(), args[1]);
	}
	else
	{
		if (server->chanModeIsSet(TOPIC_PROTECTED, args[1]) && !server->clientIsOperator(it->first, args[1]))
			Rep().E482(it->first, it->second.getNickname(), args[1]);
		else
		{
			std::string	newTopic;
			if (args.size() == 3 && args[2].size() == 1 && args[2][0] == ':')
				chan->setTopic(true, "\0");
			else
			{
				for (unsigned long i = 2; i < args.size(); i++)
				{
					newTopic.append(args[i]);
					if (i + 1 < args.size())
						newTopic.append(" ");
				}
				chan->setTopic(true, newTopic);
				chan->rplTopic();
			}
		}
	}
}

TOPIC::TOPIC(void) {}
TOPIC::~TOPIC(void) {}