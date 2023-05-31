#include "../../includes/Core/Server.hpp"

//////**********************************//////
//////              PUBLIC              //////
//////**********************************//////

//////////////////////////
//      FUNCTIONS       //
//////////////////////////

void    Server::launch(void)
{
	while (1)
	{
		fd_set tmp_fds = this->readfds;
		if (select(FD_SETSIZE, &tmp_fds, NULL, NULL, NULL) < 0)
			throw(std::runtime_error("Error: select loop failed"));
		if (FD_ISSET(mSockfd, &tmp_fds))
		    newClient(this->readfds);
		for (clientIt it = mClientsList.begin(); it != mClientsList.end(); it++)
		{
			if (FD_ISSET(it->first, &tmp_fds))
			{
				char buffer[1024];
				int num_bytes = recv(it->first, buffer, 1024, 0);
				buffer[num_bytes] = 0;
				dprintf(1, "buffer: %s", buffer);
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
				{
					std::cout << "recv failed" << std::endl;
					break ;
				}
				else if (num_bytes == 0)
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
					std::istringstream iss(msg);
					std::string line;
					while (it->second.extractMessageFromBuffer(msg))
					{
						std::istringstream stream(msg);
						while (std::getline(stream, line, ' '))
						    {applyCommand(line, msg, it, this->readfds);}
					}
				}
			}
			if (read(it->first, NULL, 0) == -1)break; 
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

bool	Server::chanAuthentication(std::string channel, std::string pwd, int clientSockfd)	const
{
	constChannelIt	itChan = mChannelList.find(channel);

	if (itChan->second.checkSpace() == false)
	{
		std::cout << "NO SPACE LEFT" << std::endl;
		return (false);
	}
	else if (itChan->second.findInvite(clientSockfd) == false)
	{
		std::cout << "NOT INVITED" << std::endl;
		return (false);
	}
	else if (itChan->second.checkPwd(pwd) == false)
	{
		std::cout << "BAD PWD OR NO PWD NEEDED" << std::endl;
		return (false);
	}
	return (true);
}

void	Server::createChan(std::string name, int clientSockfd, Client &clientData, std::string pwd, bool isPwd)
{
	Channel	newChannel(name, clientSockfd, clientData, isPwd, pwd);

	mChannelList.insert(std::pair<std::string, Channel>(name, newChannel));
	for(channelIt it = mChannelList.begin(); it != mChannelList.end(); it++)
		std::cout << it->first << std::endl;
}

void Server::joinChan(std::string name, int clientSockfd, Client &clientData)
{
	channelIt it = mChannelList.find(name);
	it->second.addClient(clientSockfd, clientData);
}

//////////////////////////
//      BUILDERS        //
//////////////////////////

Server::Server(char *port, char *pwd) : mSockfd(socket(AF_INET, SOCK_STREAM, 0)), mOptval(1), mSpecialPwd("2708")
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

	this->initCommands();
    std::cout << "Server is listening" << std::endl;
}

Server::~Server(void)
{
	std::cout << "Destructor called" << std::endl;
	//std::map<std::string, ACmd *>::iterator	it = mCmdList.begin();

    if (mSockfd >= 0)
	{
        close(mSockfd);
	}
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
	mCmdList["QUIT"] = new QUIT();
	mCmdList["PASS"] = new PASS();
	mCmdList["JOIN"] = new JOIN();
	mCmdList["NICK"] = new NICK();
	mCmdList["USER"] = new USER();
	mCmdList["PRIVMSG"] = new PRIVMSG();
	mCmdList["NOTICE"] = new NOTICE();
	// mCmdList.insert(std::pair<std::string, ACmd *>("QUIT", new QUIT()));
	// mCmdList.insert(std::pair<std::string, ACmd *>("PASS", new PASS()));
	// mCmdList.insert(std::pair<std::string, ACmd *>("NICK", new NICK()));
	// mCmdList.insert(std::pair<std::string, ACmd *>("USER", new USER()));
	// mCmdList.insert(std::pair<std::string, ACmd *>("JOIN", new JOIN()));
	// mCmdList.insert(std::pair<std::string, ACmd *>("PRIVMSG", new PRIVMSG()));
}

void Server::newClient(fd_set &readfds)
{
	Client	newClient;
	int		newClientFd = accept(mSockfd, (struct sockaddr *) &newClient.getStruct(), &newClient.getClilen());
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
	int	start = 0;
	int end = cmd.find(" ");

	while (end != -1)
	{
		args.push_back(cmd.substr(start, end - start));
		start = end + 1;
		end = cmd.find(" ", start);
	}
	args.push_back(cmd.substr(start, cmd.size() - start));
	return (args);
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
			if (it->second.getWarnLevel() >= 3 || iterator->second == 3)
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
			else if (it->second.getWarnLevel() == 2)
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
