#include "../includes/Server.hpp"

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
		for (clientIt it = mClientList.begin(); it != mClientList.end(); it++)
		{
			if (FD_ISSET(it->first, &tmp_fds))
			{
				char buffer[1024];
				int num_bytes = recv(it->first, buffer, sizeof(buffer), 0);
				buffer[num_bytes] = 0;
				//dprintf(1, "%s", buffer);
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
					mClientList.erase(it);
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
    std::cout << "Server is listening" << std::endl;
}

Server::~Server(void)
{
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

void Server::newClient(fd_set &readfds)
{
	Client	newClient;
	int		newClientFd = accept(mSockfd, (struct sockaddr *) &newClient.getStruct(), &newClient.getClilen());

	mClientList.insert (std::pair<int,Client>(newClientFd,newClient));
	FD_SET(newClientFd, &readfds);

	std::cout << "New connection from " << inet_ntoa(newClient.getAddr()) << '/' << ntohs(newClient.getPort()) << std::endl;
}

void	Server::sendMessage(int fd, std::string message)
{
	if (read(fd, 0, 0) < 0)
		return ;
	send(fd, message.append("\n").c_str(), message.size() + 1, 0);
}

bool	Server::checkExistingChannels(std::string name)
{
	if (name.find(' ') != std::string::npos)
		name.erase(0, name.find(' ') + 1);
	if (name.find('\r') != std::string::npos)
		name.erase(name.find('\r'));
	std::cout << name << std::endl;
	for (std::map<Channel,Client>::iterator it = mChannelList.begin(); it != mChannelList.end(); it++)
	{
		if (!it->first.getName().empty() && it->first.getName() == name)
				return true;
	}
	return false;
}

bool	Server::kickClient(fd_set &readfds, int fd, std::string arguments, std::string username)
{
	try
	{
		QUIT	parser(arguments, mClientList, username);
		parser.sendQuitMessage();
		close(fd);
		FD_CLR(fd, &readfds);
	}
	catch (const std::exception &e)
	{
		sendMessage(fd, e.what());
		return false;
	}
	return true;
}

void	Server::applyCommand(std::string line, std::string message, clientIt it, fd_set &readfds)
{
	if (line == "PASS")
	{PASS(message, it->second, mPwd, it->first);return;}
	else if (line == "NICK" && it->second.getAuthentication())
	{NICK(message, it->second, it->first, mClientList);return;}
	else if (line == "USER" && it->second.getAuthentication() && it->second.getNiBool() && !it->second.getUsBool())
	{USER(message, it->second, it->first, mClientList);return;}
	else if (line == "QUIT")
	{
		if (kickClient(readfds, it->first, message, it->second.getUsername()))
			mClientList.erase(it);
	}
	else {return;}
}

////////////////////////////
//		REGISTRATION	  //
////////////////////////////

//////////////////////////
//		BUILDERS		//
//////////////////////////

Server::Server(Server const &rSrc)	{(void)rSrc;}
Server::Server(void)    {}

//////////////////////////////////
//      OPERATORS OVERLOAD      //
//////////////////////////////////

Server	&Server::operator=(Server const &rRhs)	{(void)rRhs; return (*this);}
