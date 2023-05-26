#pragma once

#include "Channel.hpp"

class Server
{
    typedef std::map<int, Client>::iterator clientIt;

    public:

        void    launch(void);

        Server(char *port, char *pwd);
        ~Server(void);

    private:

        std::string mPwd;
        int         mServerPort;
        int         mSockfd;
        int                     mOptval;
        struct      sockaddr_in mServAddr;
        std::map<int,Client>    mClientsList;

        bool    portVerif(char *str) const;
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

        Server(void);
        Server(Server const &rSrc);
        Server  &operator=(Server const & rRhs);
};
