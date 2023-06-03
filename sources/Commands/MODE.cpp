#include "../../includes/Commands/Commands.hpp"

void	MODE::execute(Server *server, clientIt &it, std::vector<std::string> args)
{
	if (args.size() < 2)
	{
		Rep().E461(it->first, it->second.getNickname(), "MODE");
		return ;
	}
	else if (args[1].size() < 2 || args[1][0] != '#')
	{
		Rep().E476(it->first, args[1]);
		return ;
	}
	Channel	*chan = server->findChannel(args[1]);
	if (!chan)
		Rep().E403(it->first, it->second.getNickname(), args[1]);
	else if (server->clientIsInChannel(it->first, args[1]) == false)
		Rep().E442(it->first, it->second.getNickname(), args[1]);
	else if (args.size() == 2)
		Rep().R324(it->first, it->second.getNickname(), args[1], chan->getMods(), "");
	else if (server->clientIsOperator(it->first, args[1]) == false)
		Rep().E482(it->first, it->second.getNickname(), args[1]);
	else
	{
		bool	type = true;

		for (unsigned long i = 0; i < args[2].size(); i++)
		{
			switch (args[2][i])
			{
				case '+':
					type = true;;
					break ;
				case  '-':
					type = false;
					break ;
				case 'i':
					chan->setInviteOnly(type);
					if (type == true)
						Rep().send_to_client("MODE " + args[1] + " +i", it->first);
					else
						Rep().send_to_client("MODE " + args[1] + " -i", it->first);
					break ;
				case 't':
					chan->setTopicProtected(type);
					if (type == true)
						Rep().send_to_client("MODE " + args[1] + " +t", it->first);
					else
						Rep().send_to_client("MODE " + args[1] + " -t", it->first);
					break ;
				case 'k':
					if (type == true)
					{
						if ((args[2].size() == 1 || (args[2].size() == 2 && (args[2][0] == '+' || args[2][0] == '-'))) && args.size() == 4)
						{
							chan->setPass(type, args[3]);
							Rep().send_to_client("MODE " + args[1] + " +k " + args[3], it->first);
						}
						else
							Rep().E501(it->first, it->second.getNickname());
					}
					else
					{
						chan->setPass(type, NO_PWD);
						Rep().send_to_client("MODE " + args[1] + " -k", it->first);
					}
					break ;
				case 'o':
					if ((args[2].size() == 1 || (args[2].size() == 2 && (args[2][0] == '+' || args[2][0] == '-'))) && args.size() == 4)
					{
						int err = chan->setOperator(type, args[3]);
						if (err == -1)
							Rep().E441(it->first, it->second.getNickname(), args[1], args[3]);
						else
						{
							if (type == true)
								Rep().send_to_client("MODE " + args[1] + " +o " + args[3], it->first);
							else
								Rep().send_to_client("MODE " + args[1] + " -o " + args[3], it->first);
						}
					}
					else
						Rep().E501(it->first, it->second.getNickname());
					break ;
				case 'l':
					if (type == true)
					{
						if ((args[2].size() == 1 || (args[2].size() == 2 && (args[2][0] == '+' || args[2][0] == '-'))) && args.size() == 4)
						{
							chan->setCapped(type, std::atoi(args[3].c_str()));
							Rep().send_to_client("MODE " + args[1] + " +l " + args[3], it->first);
						}
						else
							Rep().E501(it->first, it->second.getNickname());
					}
					else
					{
						chan->setCapped(type, 0);
						Rep().send_to_client("MODE " + args[1] + " -l", it->first);
					}
					break ;
				default:
					Rep().E472(it->first, it->second.getNickname(), args[2][i]);
			}
		}
	}
}

MODE::MODE(void) {}
MODE::~MODE(void) {}
