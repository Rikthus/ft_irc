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

        std::string getPwd() {return mPwd;}
        fd_set  &getReadFds() {return readfds;}
        std::map<int,Client>    &getClientList() {return mClientsList;}

        Server(char *port, char *pwd);
        ~Server(void);

    private:

        fd_set                          readfds;
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

        std::vector<std::string>    splitCommand(std::string cmd);
};
