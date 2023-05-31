#pragma once

# include "ft_irc.hpp"
# include "RPL.hpp"
# include "Channel.hpp"
# include "Client.hpp"
# include "../Commands/Commands.hpp"

class ACmd;

class Server
{
    public:

        void    	launch(void);

		Channel 	*findChannel(std::string toFind);
        bool		clientIsInChannel(int toFind, std::string channelName);
		bool		clientIsOperator(int toFind, std::string channelName);
        bool		chanAuthentication(std::string channel, std::string pwd, int clientSockfd, std::string clientNick) const;
        bool        chanModeIsSet(int mode, std::string channelName) const;
        int         findNickSockfd(std::string nick);

		void	    joinChan(std::string name, int clientSockfd, Client &clientData);
		void	    createChan(std::string name, int clientSockfd, Client &clientData, std::string pwd, bool isPwd);
        void        setBotFd(int fd) {this->botFd = fd;}

        int         getFd() {return mSockfd;}
        int         getBotFd() {return botFd;}
        std::string getPwd() {return mPwd;}
        std::string getSpecialPwd() {return mSpecialPwd;}
        fd_set      &getReadFds() {return readfds;}
        std::map<int,Client>			&getClientList() {return mClientsList;}
        std::map<std::string,Channel>	&getChannelList() {return mChannelList;}

        Server(char *port, char *pwd);
        ~Server(void);

    private:

        fd_set                          readfds;
        int								mSockfd;
        std::string						mPwd;
        int								mOptval;
        std::string                     mSpecialPwd;
        int								mServerPort;
        int                             botFd;
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

        void    findCulprit(std::map<std::string,int>::iterator it);
       
	    std::map<std::string,int>	parseBotMessage(char *message);
        std::vector<std::string>	splitCommand(std::string cmd);
};
