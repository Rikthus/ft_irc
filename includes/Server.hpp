#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <map>

# include "Client.hpp"

# define MAX_CLIENTS 10

class Server
{
    public:

        void    launch(void);

        Server(char *port, char *pwd);
        ~Server(void);

    private:

        std::string				mPwd;
        int						mServerPort;
        int						mServerSocketFd;

        std::map<int, Client &>	mClientList;

        bool	portVerif(char *str) const;
		void	clientHandling(void);

        Server(void);
        Server(Server const &rSrc);
        Server  &operator=(Server const & rRhs);

};

#endif
