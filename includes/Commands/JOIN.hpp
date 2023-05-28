#pragma once

# include "../Core/Server.hpp"
# include "../Core/ft_irc.hpp"

class Server;

class ACmd
{
	public:
		virtual	void	execute(Server *server, int clientSockfd, Client &clientData, std::vector<std::string> args) = 0;
		ACmd(void);
		virtual	~ACmd(void);
};

class JOIN: public ACmd
{
	public:
		void execute(Server *server, int clientSockfd, Client &clientData, std::vector<std::string> args);

		JOIN(void);
		~JOIN(void);
	private:
		void	joinChan(Server *server, int clientSockfd, Client &clientData);
		void	createChan(Server *server, int clientSockfd, Client &clientData);
};
