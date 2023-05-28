#pragma once

#include "Channel.hpp"
#include "QUIT.hpp"
#include "PASS.hpp"
#include "NICK.hpp"
#include "USER.hpp"

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
        std::map<int,Client>    mClientList;
        std::map<Channel,Client> mChannelList;

        bool    portVerif(char *str) const;
        void    newClient(fd_set &readfds);
        void    sendMessage(int fd, std::string message);
		void	applyCommand(std::string line, std::string message, clientIt it, fd_set &readfds);

        bool    kickClient(fd_set &readfds, int fd, std::string arguments, std::string username);

        bool    checkExistingChannels(std::string name);

        Server(void);
        Server(Server const &rSrc);
        Server  &operator=(Server const & rRhs);
};
