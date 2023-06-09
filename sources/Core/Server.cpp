#include "../../includes/Core/Server.hpp"

//////**********************************//////
//////              PUBLIC              //////
//////**********************************//////

//////////////////////////
//      FUNCTIONS       //
//////////////////////////

int         globalSignal = 0;

void	exitHandler(int signal)
{
	(void) signal;
	globalSignal = 1;
}

void    Server::launch(void)
{
	while (1)
	{
		fd_set tmp_fds = this->readfds;
		if (sigaction(SIGINT, &signalHandler, 0) == -1)
			throw(std::runtime_error("Error: signal handler failed"));
		if (globalSignal == 1)
			return ;
		if (select(FD_SETSIZE, &tmp_fds, 0, 0, 0) < 0)
			throw(std::runtime_error("Error: select failed"));
		if (FD_ISSET(mSockfd, &tmp_fds))
		    newClient(this->readfds);
		for (clientIt it = mClientsList.begin(); it != mClientsList.end(); it++)
		{
			int	tmpSockfd = it->first;
			if (FD_ISSET(it->first, &tmp_fds))
			{
				char buffer[1024];
				int num_bytes = recv(it->first, buffer, 1024, 0);
				buffer[num_bytes] = 0;
				std::string	buf = buffer;
				std::cout << "buffer: " << buf;
				if (buf.size() > 1023)
					break ;
				if (it->second.getUsername() == "[Mildred]" && num_bytes > 0)
				{
					std::map<std::string,int>	culprit;
					culprit = parseBotMessage(buffer);
					std::map<std::string,int>::iterator it = culprit.begin();
					if (it->second > 0)
						findCulprit(it);
					break ;
				}
				if (num_bytes < 0)
					break ;
				else if (num_bytes == 0 && buf.find("QUIT") == std::string::npos)
				{
					std::cout << "Client " << ntohs(it->second.getPort()) << " disconnected" << std::endl;
					close(it->first);
					FD_CLR(it->first, &this->readfds);
					mClientsList.erase(it);
					break ;
				}
				else
				{
					it->second.appendToBuffer(buffer, num_bytes);
					std::string	msg;
					std::string line;
					while (it->second.extractMessageFromBuffer(msg, tmpSockfd))
					{
						std::istringstream stream(msg);
						while (std::getline(stream, line, ' '))
						{
							if (read(tmpSockfd, NULL, 0) == -1)
								break;
							applyCommand(line, msg, it, this->readfds);
						}
					}
				}
			}
			if (read(tmpSockfd, NULL, 0) == -1)
			{break;}
		}
	}
}

Channel *Server::findChannel(std::string toFind)
{
	channelIt	it = mChannelList.find(toFind);
	if (it != mChannelList.end())
		return (&(it->second));
	else
		return (NULL);
}

bool	Server::clientIsInChannel(int toFind, std::string channelName)
{
	channelIt	it;
	std::map<int, Client *>::iterator	itMap;
	Channel			*chan;

	it = mChannelList.find(channelName);
	if (it == mChannelList.end())
		return (false);
	chan = this->findChannel(channelName);
	if (chan->sockClientIsInChan(toFind))
		return (true);
	else
		return (false);
}

bool	Server::clientIsOperator(int toFind, std::string channelName)
{
	channelIt	it;
	std::map<int, Client *>::iterator	itMap;
	Channel			*chan;

	it = mChannelList.find(channelName);
	if (it == mChannelList.end())
		return (false);
	chan = this->findChannel(channelName);
	if (chan->sockClientIsOperator(toFind))
		return (true);
	else
		return (false);
}

bool	Server::chanAuthentication(std::string channel, std::string pwd, int clientSockfd, std::string clientNick)
{
	channelIt	itChan = mChannelList.find(channel);

	if (itChan->second.checkSpace() == false)
	{
		Rep().E471(clientSockfd, clientNick, channel);
		return (false);
	}
	else if (itChan->second.checkPwd(pwd) == false)
	{
		Rep().E475(clientSockfd, clientNick, channel);
		return (false);
	}
	else if (itChan->second.findInvite(clientSockfd) == false)
	{
		Rep().E473(clientSockfd, clientNick, channel);
		return (false);
	}
	return (true);
}

bool	Server::chanModeIsSet(int mode, std::string chanName) const
{
	constChannelIt	it = mChannelList.find(chanName);

	switch (mode)
	{
		case TOPIC_PROTECTED:
			return (it->second.getTopicProtected());
		case PASS_PROTECTED:
			return (it->second.getPassProtected());
		case INVITE_ONLY:
			return (it->second.getInviteOnly());
		case CAPPED:
			return (it->second.getCapped());
		case TOPIC_SET:
			return (it->second.getTopicSet());
	}
	return (false);
}

int	Server::findNickSockfd(std::string nick)
{
	clientIt	it;

	for(it = mClientsList.begin(); it != mClientsList.end(); it++)
	{
		if (it->second.getNickname() == nick)
			return (it->second.getFd());
	}
	return (-1);
}

void	Server::createChan(std::string name, int clientSockfd, Client &clientData, std::string pwd, bool isPwd)
{
	Channel	newChannel(name, clientSockfd, clientData, isPwd, pwd);

	mChannelList.insert(std::pair<std::string, Channel>(name, newChannel));
}

void Server::joinChan(std::string name, int clientSockfd, Client &clientData)
{
	channelIt it = mChannelList.find(name);
	it->second.addClient(clientSockfd, clientData);
}

//////////////////////////
//      BUILDERS        //
//////////////////////////

Server::Server(char *port, char *pwd) : mSockfd(socket(AF_INET, SOCK_STREAM, 0)), mOptval(1), mSpecialPwd(SpecialPwd), botFd(-1)
{
    std::cout << "Server constructor engaged" << std::endl;
	FD_ZERO(&readfds);FD_SET(mSockfd, &readfds);
    if (mSockfd < 0)
        throw(std::runtime_error("Error: socket attribution failed"));
    if (setsockopt(mSockfd, SOL_SOCKET, SO_REUSEPORT, &mOptval, sizeof(mOptval)) < 0)
        throw(std::runtime_error("Error: socket option attribution failed"));
    mServAddr.sin_family = AF_INET;
    mServAddr.sin_addr.s_addr = INADDR_ANY;
	if (this->portVerif(port))
	{
		mServerPort = std::atoi(port);
		mPwd = pwd;
        mServAddr.sin_port = htons(mServerPort);
	}
	else
		throw(std::runtime_error("Error: invalid port (6660-6669 or 7000)"));
    if (bind(mSockfd, (struct sockaddr *) &mServAddr, sizeof(mServAddr)) < 0)
        throw(std::runtime_error("Error: socket binding failed"));
    if (listen(mSockfd, 5) < 0)
	{
        throw(std::runtime_error("Error: server launch failed"));
	}
	signalHandler.sa_handler = exitHandler;
	sigemptyset(&signalHandler.sa_mask);
	signalHandler.sa_flags = 0;
	this->initCommands();
    std::cout << "Server is listening" << std::endl;
	pigeonBuilder();
}

Server::~Server(void)
{
	for (std::map<std::string, ACmd *>::iterator it = mCmdList.begin(); it != mCmdList.end(); it++)
	 	delete it->second;
	// mCmdList.erase(mCmdList.begin(), mCmdList.end());
	for (std::map<int, Client>::iterator it = mClientsList.begin(); it != mClientsList.end(); it++)
	{
		if (it->second.getNickname() == "[Mildred]")
			send(it->first, "[FROM]_[server]_SHUTDOWN\r\n", 27, 0);
		// else
		// {
		// 	close(it->first);
		// 	FD_CLR(it->first, &readfds);
		// }
	}
	//mClientsList.erase(mClientsList.begin(), mClientsList.end());
	FD_CLR(mSockfd, &readfds);
	close(mSockfd);
}


//////**********************************//////
//////              PRIVATE             //////
//////**********************************//////

//////////////////////////
//		FUNCTIONS		//
//////////////////////////

bool Server::portVerif(char *str)	const
{
    int value;

    for (int i = 0; str[i]; i++)
    {
        if (i > 3 || !std::isdigit(str[i]))
            return (false);
    }
    value = std::atoi(str);
    if ((value < 6660 || value > 6669) && value != 7000)
        return (false);
    return (true);
}

void	Server::initCommands(void)
{
	mCmdList["INVITE"] =	new INVITE();
	mCmdList["JOIN"] =		new JOIN();
	mCmdList["KICK"] =		new KICK();
	mCmdList["MODE"] =		new MODE();
	mCmdList["NICK"] =		new NICK();
	mCmdList["NOTICE"] =	new NOTICE();
	mCmdList["PASS"] =		new PASS();
	mCmdList["PRIVMSG"] =	new PRIVMSG();
	mCmdList["QUIT"] =		new QUIT();
	mCmdList["TOPIC"] =		new TOPIC();
	mCmdList["USER"] =		new USER();
}

void Server::newClient(fd_set &readfds)
{
	Client	newClient;
	int		newClientFd = accept(mSockfd, (struct sockaddr *) &newClient.getStruct(), &newClient.getClilen());
	if (newClientFd == -1)
		throw(std::runtime_error("Error: cannot accept new client connection"));
	newClient.setClifd(newClientFd);
	mClientsList.insert (std::pair<int,Client>(newClientFd,newClient));
	FD_SET(newClientFd, &readfds);

	std::cout << "New connection from " << inet_ntoa(newClient.getAddr()) << '/' << ntohs(newClient.getPort()) << std::endl;
}

void	Server::sendMessage(int fd, std::string message)
{send(fd, message.append("\n").c_str(), message.size() + 1, 0);}

void	Server::applyCommand(std::string line, std::string message, clientIt it, fd_set &readfds)
{
	(void) readfds;
	int		tmpSockfd = it->first;
	if (read(tmpSockfd, NULL, 0) == -1)
		return ;
	std::map<std::string, ACmd *>::iterator	itCmd = mCmdList.find(line);
	if (itCmd != mCmdList.end())
	{
		std::vector<std::string>	args;
		for (int i = 0; message[i]; i++)
		{
			if (message[i] != ' ' && std::isspace(message[i]))
				message.erase(i, 1);
			else if (message[i] == ' ' && message[i + 1] == ' ')
				message.erase(i--, 1);
		}
		args = splitCommand(message);
		itCmd->second->execute(this, it, args);
		return ;
	}
}

std::vector<std::string>	Server::splitCommand(std::string cmd)
{
	std::vector<std::string>	args;
	std::string	line;
	std::istringstream iss(cmd);

	while (std::getline(iss, line, ' '))
	{
		if (!line.empty())
			args.push_back(line);
	}
	return args;
}

//////////////
//	MILDRED	//
//////////////

std::map<std::string,int>	Server::parseBotMessage(char *message)
{
	std::string					buffer = message;
	std::map<std::string,int>	culprit;
	int						warnLevel = 0;
	if (buffer.find("[FROM_BOT]_") != std::string::npos)
		buffer.erase(0, 11);
	warnLevel += std::atoi(buffer.substr(buffer.find("->") + 2, std::string::npos).c_str());
	buffer = buffer.substr(0, buffer.find("->"));
	culprit.insert (std::pair<std::string,int>(buffer,warnLevel));
	return culprit;
}

void	Server::findCulprit(std::map<std::string,int>::iterator iterator)
{
	for (std::map<int,Client>::iterator it = mClientsList.begin(); it != mClientsList.end(); it++)
	{
		if (it->second.getNickname() == iterator->first)
		{
			it->second.addWarns(iterator->second);
			std::string	warning;
			if (iterator->second == 3)
			{
				warning = ":[Mildred] PRIVMSG " + it->second.getNickname() + " :You deserve a fate worse than death. Begone.\r\n";
				send(it->first, warning.c_str(), warning.size(), 0);
				close(it->first);
				FD_CLR(it->first, &readfds);
				mClientsList.erase(it);
			}
			else if (it->second.getWarnLevel() == 1)
			{
				warning = ":[Mildred] PRIVMSG " + it->second.getNickname() + " :What did you just say? Chill the fuck down, first warning.\r\n";
				send(it->first, warning.c_str(), warning.size(), 0);
			}
			else if (it->second.getWarnLevel() == 2)
			{
				warning = ":[Mildred] PRIVMSG " + it->second.getNickname() + " :I just nicely asked you to calm down, second warning.\r\n";
				send(it->first, warning.c_str(), warning.size(), 0);
			}
			else if (it->second.getWarnLevel() == 3)
			{
				warning = ":[Mildred] PRIVMSG " + it->second.getNickname() + " :Aight bro, that's it, bye.\r\n";
				send(it->first, warning.c_str(), warning.size(), 0);
				close(it->first);
				FD_CLR(it->first, &readfds);
				mClientsList.erase(it);
			}
			return ;
		}
	}
}

void    Server::pigeonBuilder()
{
    std::string parts;

    parts = "                   ░░░░\r\n";
    parts += "              ░░░░░░░░▒▒░░░░░░\r\n";
    parts += "          ░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░\r\n";
    parts += "        ░░░░▒▒▒▒▒▒▒▒▒▒░░░░▒▒▒▒▒▒░░\r\n";
    parts += "      ░░░░▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒░░▒▒▒▒▒▒\r\n";
    parts += "      ░░░░▒▒▒▒▒▒▒▒▒▒░░  ▓▓▒▒░░▒▒▒▒▒▒\r\n";
    parts += "      ▒▒░░░░▒▒▒▒▒▒▒▒░░▓▓▓▓▓▓░░▒▒▒▒▒▒▒▒\r\n";
    parts += "    ░░░░░░░░░░░░▒▒▒▒░░▒▒▓▓▓▓░░▒▒▒▒▒▒▒▒\r\n";
    parts += "  ░░░░▒▒▓▓░░░░░░▒▒▒▒▒▒░░░░░░▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "  ░░▒▒▒▒▓▓▒▒░░░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "  ▒▒░░▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░\r\n";
    parts += "  ░░░░▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "  ▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓\r\n";
    parts += "░░░░▒▒▓▓▓▓▓▓████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░\r\n";
    parts += "░░▒▒▓▓████████▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓\r\n";
    parts += "  ▒▒          ▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓\r\n";
    parts += "              ▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "              ░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓\r\n";
    parts += "                ▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓\r\n";
    parts += "                ▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "                ▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░\r\n";
    parts += "                ▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "              ░░▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "              ░░▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "              ▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "              ▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "              ▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▓▓▓▓▓▓▓▓░░\r\n";
    parts += "              ▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒░░░░░░░░\r\n";
    parts += "            ▒▒▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▒▒▒▒▓▓▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░\r\n";
    parts += "            ▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░▒▒░░\r\n";
    parts += "          ▒▒▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▒▒▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▓▓▒▒░░\r\n";
    parts += "          ▓▓▓▓▓▓▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓▓▓▓▓▒▒░░                                                        ░░\r\n";
    parts += "        ░░▓▓▓▓▒▒▓▓▒▒▓▓▓▓▒▒▓▓▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓▓▓▓▓▓▓░░░░░░░░░░                                      ▒▒██▓▓▓▓▓▓\r\n";
    parts += "        ▓▓▓▓▓▓▒▒▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓▒▒▓▓▒▒▒▒▒▒▓▓▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▒▒▓▓▓▓░░░░░░░░░░░░▓▓▒▒░░░░                  ░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓\r\n";
    parts += "        ▓▓▓▓▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓▒▒▓▓▒▒▓▓▒▒▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░    ░░░░░░▒▒▒▒▓▓▓▓▓▓▒▒░░░░░░░░░░░░▒▒▓▓▓▓░░░░░░░░░░░░  ▒▒▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓▒▒\r\n";
    parts += "        ▓▓▓▓▒▒▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓▒▒▒▒▓▓▒▒▒▒▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▓▓▓▓░░░░░░░░  ░░░░  ░░▓▓▒▒░░░░░░░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓██░░\r\n";
    parts += "      ░░▓▓▒▒▒▒▓▓▒▒▓▓▓▓▒▒▓▓▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░  ░░░░░░░░░░░░░░░░░░░░░░▒▒▓▓▒▒░░░░░░░░░░░░░░▒▒▓▓░░░░░░░░░░░░▒▒▓▓▓▓▓▓▓▓▒▒▓▓▓▓▓▓\r\n";
    parts += "      ░░▓▓▒▒▒▒▓▓▒▒▓▓▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  ▓▓▒▒░░░░░░░░░░░░▒▒▓▓▓▓▒▒▒▒░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓\r\n";
    parts += "        ░░▒▒▒▒▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▓▓▓▓░░░░░░░░░░░░░░▒▒▓▓▓▓▓▓░░▒▒██▓▓▓▓▓▓▓▓▓▓██▓▓████▓▓▓▓▒▒\r\n";
    parts += "          ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░▒▒▓▓▓▓░░░░░░▓▓▓▓▓▓████▓▓▓▓▓▓▓▓▓▓▒▒\r\n";
    parts += "          ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓▓▓▒▒░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░  ░░▓▓▓▓▓▓▓▓▓▓░░░░░░░░▒▒▓▓▓▓░░░░░░▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒\r\n";
    parts += "          ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▒▒▒▒▓▓░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░▒▒▓▓▒▒▒▒░░░░░░░░░░░░▓▓▓▓▓▓▓▓▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒\r\n";
    parts += "          ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒  ░░░░░░░░  ░░░░░░░░░░░░░░░░▒▒▓▓▓▓▓▓▓▓▓▓░░░░░░░░░░░░░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒\r\n";
    parts += "            ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒░░░░▒▒░░░░░░░░░░░░░░░░░░░░░░░░▒▒▓▓▓▓░░░░░░░░░░░░░░░░░░▒▒▓▓▓▓██▓▓▓▓▓▓▓▓██▒▒\r\n";
    parts += "            ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▒▒▒▒▓▓▓▓░░  ░░░░░░░░░░░░░░░░░░▒▒░░░░░░░░▓▓▓▓▓▓▓▓░░░░░░░░░░░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒\r\n";
    parts += "            ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒░░░░▒▒░░░░░░░░░░░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░\r\n";
    parts += "              ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▒▒░░░░░░▒▒░░░░░░░░░░░░░░▓▓▓▓▒▒░░░░░░▒▒▓▓████▓▓▓▓████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒░░\r\n";
    parts += "              ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓░░░░░░▓▓▓▓▒▒▒▒▒▒▒▒▒▒░░░░░░░░░░░░░░░░░░▒▒▓▓▓▓▓▓████▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░\r\n";
    parts += "                ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░▓▓▓▓▓▓▒▒▒▒░░░░░░░░░░░░░░░░▒▒██▓▓▓▓▓▓▓▓▓▓██▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒\r\n";
    parts += "                  ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒░░░░░░░░░░░░▒▒▒▒▓▓██▓▓██▓▓▓▓▓▓▓▓▓▓▒▒▓▓██▓▓██▓▓▓▓▒▒░░░░\r\n";
    parts += "                    ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░░░░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▓▓██\r\n";
    parts += "                      ░░▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓  ░░▒▒▓▓▓▓▓▓▒▒▓▓▓▓▓▓▓▓██\r\n";
    parts += "                          ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░              ░░░░▒▒\r\n";
    parts += "                              ▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒\r\n";
    parts += "                                  ▒▒▓▓▒▒▒▒▒▒▒▒▒▒▓▓▒▒▓▓▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒\r\n";
    parts += "                                        ░░▒▒▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "                                                      ▒▒▒▒▒▒▒▒▒▒▒▒      ▒▒▒▒▒▒▒▒▒▒▒▒\r\n";
    parts += "                                                      ░░▒▒▒▒▒▒▒▒▒▒      ░░▒▒░░▒▒▒▒▒▒\r\n";
    parts += "                                                        ░░▒▒▒▒▒▒▒▒        ░░░░▒▒▒▒▒▒\r\n";
    parts += "                                                        ░░▒▒▒▒▒▒          ░░░░▒▒▒▒▒▒\r\n";
    parts += "                                                          ▒▒▒▒▒▒            ░░▒▒▒▒▒▒\r\n";
    parts += "                                                          ▒▒▓▓              ▒▒▒▒▒▒░░\r\n";
    parts += "                                                          ▒▒░░              ▒▒▒▒▒▒\r\n";
    parts += "                                                        ░░▒▒                ▒▒▒▒\r\n";
    parts += "                                                      ░░▒▒                  ▒▒░░\r\n";
    parts += "                                        ▒▒▒▒░░░░░░░░▒▒▒▒▒▒▒▒▓▓▓▓          ▒▒▒▒\r\n";
    parts += "                                      ▒▒▒▒▒▒▒▒▒▒░░▒▒▒▒▒▒▒▒▒▒░░            ░░▒▒░░\r\n";
    parts += "                                          ░░░░▒▒▒▒▒▒░░▒▒                ▒▒▒▒▒▒▒▒▒▒██▒▒\r\n";
    parts += "                                        ▒▒▒▒▒▒░░  ░░▒▒░░      ░░▒▒░░▒▒▒▒░░▒▒▒▒▒▒░░\r\n";
    parts += "                                      ▓▓▓▓░░      ░░▒▒░░  ░░▒▒▒▒▒▒▒▒░░▒▒▒▒░░▒▒▒▒\r\n";
    parts += "                                      ░░          ▒▒▓▓    ▓▓    ░░▒▒▒▒░░░░▒▒░░▒▒\r\n";
    parts += "                                                  ░░▒▒        ▒▒▒▒          ▒▒▒▒░░\r\n";
    parts += "                                                  ░░        ██▒▒              ▒▒▒▒\r\n";
    parts += "                                                            ▒▒\r\n";
    pigeon = parts;
}
