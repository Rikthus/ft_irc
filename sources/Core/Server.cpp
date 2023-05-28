#include "../../includes/Core/Server.hpp"

//////**********************************//////
//////              PUBLIC              //////
//////**********************************//////

//////////////////////////
//      FUNCTIONS       //
//////////////////////////

void    Server::launch(void)
{
    fd_set readfds;

	FD_ZERO(&readfds);
	FD_SET(mSockfd, &readfds);

	while (1)
	{
		fd_set tmp_fds = readfds;
		if (select(FD_SETSIZE, &tmp_fds, NULL, NULL, NULL) < 0)
			throw(std::runtime_error("Error: select loop failed"));
		if (FD_ISSET(mSockfd, &tmp_fds))
		    newClient(readfds);
		for (clientIt it = mClientsList.begin(); it != mClientsList.end(); it++)
		{
			if (FD_ISSET(it->first, &tmp_fds))
			{
				char buffer[1024];
				int num_bytes = recv(it->first, buffer, sizeof(buffer), 0);
				buffer[num_bytes] = 0;
				std::cout << buffer;
				if (num_bytes < 0)
				{
					std::cout << "recv failed" << std::endl;
					break ;
				}
				else if (num_bytes == 0)
				{
					std::cout << "Client " << ntohs(it->second.getPort()) << " disconnected" << std::endl;
					close(it->first);
					FD_CLR(it->first, &readfds);
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
						    {applyCommand(line, msg, it, readfds);}
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

//////////////////////////
//      BUILDERS        //
//////////////////////////

Server::Server(char *port, char *pwd) : mSockfd(socket(AF_INET, SOCK_STREAM, 0)), mOptval(1)
{
    std::cout << "Server constructor engaged" << std::endl;
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
        throw(std::runtime_error("Error: server launch failed"));
	this->initCommands();
    std::cout << "Server is listening" << std::endl;
}

Server::~Server(void)
{
	std::map<std::string, ACmd *>::iterator	it;

    if (mSockfd >= 0)
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
	mCmdList.insert(std::pair<std::string, ACmd *>("JOIN", new JOIN()));
}

void Server::newClient(fd_set &readfds)
{
	Client	newClient;
	int		newClientFd = accept(mSockfd, (struct sockaddr *) &newClient.getStruct(), &newClient.getClilen());

	mClientsList.insert (std::pair<int,Client>(newClientFd,newClient));
	FD_SET(newClientFd, &readfds);

	std::cout << "New connection from " << inet_ntoa(newClient.getAddr()) << '/' << ntohs(newClient.getPort()) << std::endl;
}

void	Server::sendMessage(int fd, std::string message)
{send(fd, message.append("\n").c_str(), message.size() + 1, 0);}

void	Server::applyCommand(std::string line, std::string message, clientIt it, fd_set &readfds)
{
	std::map<std::string, ACmd *>::iterator	itCmd = mCmdList.find(line);

	// if (itCmd == mCmdList.end())
	// 	std::cout << "No Command found" << std::endl;
	if (line == "PASS")
	{authenticateClient(message, it->first, it->second);return;}
	else if (line == "NICK" && it->second.getAuthentication())
	{registerClientsNick(message, it->first, it->second);return;}
	else if (line == "USER" && it->second.getAuthentication())
	{registerClientsUser(message, it->first, it->second);return;}
	else if (line == "QUIT")
	{
		std::cout << "Client " << ntohs(it->second.getPort()) << " disconnected" << std::endl;
		close(it->first);
		FD_CLR(it->first, &readfds);
		mClientsList.erase(it);
		return ;
	}
	else if (line == "JOIN")
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
		itCmd->second->execute(this, it->first, it->second, args);
		return ;
	}
}

bool	Server::checkDuplicateNick(std::string nickname)
{
	for (std::map<int,Client>::iterator it = mClientsList.begin(); it != mClientsList.end(); it++)
	{
		if (!it->second.getNickname().empty() && it->second.getNickname() == nickname)
			return true;
	}
	return false;
}

bool	Server::checkDuplicateUser(std::string username)
{
	for (std::map<int,Client>::iterator it = mClientsList.begin(); it != mClientsList.end(); it++)
	{
		if (!it->second.getUsername().empty() && it->second.getUsername() == username)
			return true;
	}
	return false;
}

bool	Server::checkCharacter(char character)
{
	std::string	validCharacters("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ0123456789");
	
	if (validCharacters.find(character) != std::string::npos)
		return false;
	else
		return true;
}

bool	Server::checkCharactersValidity(std::string name)
{
	for (size_t index = 0; index < name.size(); index++)
	{
		if (checkCharacter(name[index]))
			return true;
	}
	return false;
}

void    Server::authenticateClient(std::string msg, int fd, Client &Client)
{
    std::string password = msg.substr(msg.find(' ') + 1, msg.size());
	if (password.find('\r') != std::string::npos)
		password.erase(password.find('\r'), std::string::npos);
	if (Client.getAuthentication() == true)
		return ;
	if (password == mPwd)
		Client.setAuthentication();
	else
		sendMessage(fd, "Incorrect password");
}

void	Server::registerClientsNick(std::string msg, int fd, Client &Client)
{
	std::string name = msg.substr(msg.find(' ') + 1, msg.size());

	if (name.find('\r') != std::string::npos)
		name.erase(name.find('\r'), std::string::npos);
	if (checkCharactersValidity(name))
		return ;
	if (!checkDuplicateNick(name))
	{
		Client.setNickname(name);
	 	std::string	confirmation("Your nickname is set to "); confirmation.append(Client.getNickname());
	}
	else
		sendMessage(fd, "Nickname already taken");
}

void	Server::registerClientsUser(std::string msg, int fd, Client &Client)
{
	std::string name = msg.erase(0, msg.find(' ') + 1);

	if (name.find(' ') != std::string::npos)
		name.erase(name.find(' '), std::string::npos);
	if (checkCharactersValidity(name))
		return ;
	if (!checkDuplicateUser(name))
	{
		Client.setUsername(name);
	 	std::string	confirmation("Your username is set to "); confirmation.append(Client.getUsername());
	}
	else
		sendMessage(fd, "Username already taken");
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
