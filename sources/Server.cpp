#include "../includes/Server.hpp"

//////**********************************//////
//////              PUBLIC              //////
//////**********************************//////

//////////////////////////
//      FUNCTIONS       //
//////////////////////////

void    Server::launch(void)
{
	int					flags;
    struct sockaddr_in	serverAddr;

	mServerSocketFd = socket(AF_INET, SOCK_STREAM, 0);
	if (mServerSocketFd == -1)
		throw(std::runtime_error("Error: cannot create server socket"));
	flags = fcntl(mServerSocketFd, F_GETFL, 0);
	if (flags == -1)
		throw(std::runtime_error("Error: cannot get server socket flags"));
	if (fcntl(mServerSocketFd, F_SETFL, flags | O_NONBLOCK) == -1)
		throw(std::runtime_error("Error: cannot change server socket flags to non blockant"));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(mServerPort);
	if (bind(mServerSocketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1)
		throw(std::runtime_error("Error: cannot bind server socket to ip/port"));
	if (listen(mServerSocketFd, MAX_CLIENTS) == -1)
		throw(std::runtime_error("Error: cannot listen to clients"));
	clientHandling();
}

//////////////////////////
//      BUILDERS        //
//////////////////////////

Server::Server(char *port, char *pwd)
{
	mServerSocketFd = -1;
	if (this->portVerif(port))
	{
		mServerPort = std::atoi(port);
		mPwd = pwd;
	}
	else
		throw(std::runtime_error("Error: invalid port (6660-6669 or 7000)"));
}

Server::~Server(void)
{
	if (mServerSocketFd != -1)
		close(mServerSocketFd);
}


//////**********************************//////
//////              PRIVATE             //////
//////**********************************//////

//////////////////////////
//		FUNCTIONS		//
//////////////////////////

bool	Server::portVerif(char *str)	const
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

void	Server::clientHandling(void)
{
	fd_set	readFds;
	int		newSocketFd;


	FD_ZERO(&readFds);
	FD_SET(mServerSocketFd, &readFds);
	while (true)
	{
		fd_set	tmpFds = readFds;
		struct	sockaddr_in	clientAddr;
		socklen_t			clilen = sizeof(clientAddr);
		
		if (select(FD_SETSIZE, &tmpFds, NULL, NULL, NULL) == -1)
			std::cerr << "Error: while selecting a socket for reading" << std::endl;
		else
		{
			for (int i = 0; i < FD_SETSIZE; i++)
			{
				if (FD_ISSET(i, &tmpFds))
				{
					if (i == mServerSocketFd)
					{
						newSocketFd = accept(mServerSocketFd, (struct sockaddr*)&clientAddr, &clilen);
						if (newSocketFd == -1)
							throw(std::runtime_error("Error: cannot accept new client socket"));
						send(newSocketFd, "Ca marche\n", 10, 0);
					}
				}
			}
		}
	}
}

//////////////////////////
//		BUILDERS		//
//////////////////////////

Server::Server(Server const &rSrc)	{(void)rSrc;}
Server::Server(void)    {}

//////////////////////////////////
//      OPERATORS OVERLOAD      //
//////////////////////////////////

Server	&Server::operator=(Server const &rRhs)	{(void)rRhs; return (*this);}
