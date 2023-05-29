#pragma once

# include "ft_irc.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "../Commands/JOIN.hpp"

class ACmd;

class Server
{
    public:

        void    	launch(void);
		Channel 	*findChannel(std::string toFind);
		bool		clientIsInChannel(int toFind, std::string channelName) const;
		bool		clientIsOperator(int toFind, std::string channelName) const;
        bool		chanAuthentication(std::string channel, std::string pwd, int clientSockfd) const;

		void	joinChan(std::string name, int clientSockfd, Client &clientData);
		void	createChan(std::string name, int clientSockfd, Client &clientData, std::string pwd, bool isPwd);

        Server(char *port, char *pwd);
        ~Server(void);

    private:

        std::string						mPwd;
        int								mServerPort;
        int								mSockfd;
        int								mOptval;
        struct	sockaddr_in				mServAddr;
        std::map<int,Client>			mClientsList;
		std::map<std::string, Channel>	mChannelList;
		std::map<std::string, ACmd *>	mCmdList;

        bool    portVerif(char *str) const;
		void	initCommands(void);
        void    newClient(fd_set &readfds);
        void    sendMessage(int fd, std::string message);
		void	applyCommand(std::string line, std::string message, clientIt it, fd_set &readfds);

		void	authenticateClient(std::string msg, int fd, Client &Client);
		void	registerClientsNick(std::string msg, int fd, Client &Client);
		void	registerClientsUser(std::string msg, int fd, Client &Client);

		bool	checkDuplicateNick(std::string nickname);
		bool	checkDuplicateUser(std::string username);
        bool    checkCharacter(char character);
        bool    checkCharactersValidity(std::string name);

        std::vector<std::string>    splitCommand(std::string cmd);
};
