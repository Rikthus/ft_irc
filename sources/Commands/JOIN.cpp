#include "../../includes/Commands/JOIN.hpp"
//*******************  ABSTRACT CLASS  ********************////

ACmd::ACmd(void) {}
ACmd::~ACmd(void) {}

//********************************  JOIN  ****************************////

void	JOIN::execute(Server *server, int clientSockfd, Client &clientData, std::vector<std::string> args)
{
	std::vector<std::string>	channels;
	std::vector<std::string>	keys;

	int		nbChan = 0;
	int		nbKey = 0;
	bool	possibleChan = true;
	bool	possibleKey = false;

	std::cout << "JOINED" << std::endl;
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
			std::cout << "BAD FORMAT" << std::endl;
			return ;
		}
		else if (possibleKey && nbKey < nbChan)
		{
			keys.push_back(args[i]);
			nbKey++;
		}
		else
		{
			std::cout << "BAD FORMAT" << std::endl;
			return ;
		}
	}
	for (unsigned long i = 0; i < channels.size(); i++)
	{
		if (i < keys.size())
		{
			if (server->findChannel(channels[i]))
			{
				if (server->chanAuthentication(channels[i], keys[i], clientSockfd))
				{
					server->joinChan(channels[i], clientSockfd, clientData);
					std::cout << "TRY ADD TO CHAN " << channels[i] << std::endl;
				}
				else
					std::cout << "BAD PASSORD AND/OR MISSING INVITATION AND/OR NO SPACE LEFT" << std::endl;
			}
			else
				server->createChan(channels[i], clientSockfd, clientData, keys[i], true);
		}
		else
		{
			if (server->findChannel(channels[i]))
			{
				if (server->chanAuthentication(channels[i], NO_PWD, clientSockfd))
				{
					server->joinChan(channels[i], clientSockfd, clientData);
					std::cout << "TRY ADD TO CHAN " << channels[i] << std::endl;
				}
				else
					std::cout << "MISSING INVITATION OR NO SPACE LEFT" << std::endl;
			}
			else
				server->createChan(channels[i], clientSockfd, clientData, NO_PWD, false);
		}
	}
}

JOIN::JOIN(void) {}

JOIN::~JOIN(void) {}
