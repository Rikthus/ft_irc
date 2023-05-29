#pragma once

# include "../Core/ft_irc.hpp"
# include "../Core/Server.hpp"

class Server;

class ACmd
{
	public:
		virtual	void	execute(Server *server, clientIt &iterator, std::vector<std::string> args) = 0;
		ACmd(void);
		virtual	~ACmd(void);
};

class PASS: public ACmd
{
	public:
		PASS();
		~PASS();
		void	execute(Server *server, clientIt &iterator, std::vector<std::string> args);
	private:
		const std::string type;
};

class JOIN: public ACmd
{
	public:

		JOIN(void);
		~JOIN(void);
		void execute(Server *server, clientIt &iterator, std::vector<std::string> args);
	private:
		void	joinChan(Server *server, int clientSockfd, Client &clientData);
		void	createChan(Server *server, int clientSockfd, Client &clientData);
};

class QUIT: public ACmd
{
	public:
		QUIT();
		~QUIT();
		void	execute(Server *server, clientIt &iterator, std::vector<std::string> args);

	private:
		const		std::string type;
};

class NICK: public ACmd
{
	public:
		NICK();
		~NICK();
		bool	checkDuplicateNick(std::string nickname, std::map<int,Client> mClientList);
		void	execute(Server *server, clientIt &iterator, std::vector<std::string> args);

	private:
		const		std::string type;
};

class USER: public ACmd
{
	public:
		USER();
		~USER();
		bool	checkDuplicateUser(std::string username, std::map<int,Client> mClientList);
		void	execute(Server *server, clientIt &iterator, std::vector<std::string> args);

	private:
		const		std::string type;
};

class PRIVMSG: public ACmd
{
	public:
		PRIVMSG();
		~PRIVMSG();
		int		findUser(std::map<int,Client> clientList, std::string name);
		void	execute(Server *server, clientIt &iterator, std::vector<std::string> args);

	private:
		const		std::string type;
};

bool    checkCharacter(char character);
bool    checkCharactersValidity(std::string name);