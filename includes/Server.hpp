#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <sys/types.h>

class Server
{
    public:

        Server(char *port, char *pwd);
        ~Server(void);


    private:

        std::string mPwd;
        int         mServerPort;

        Server(void);
        Server(Server const &rSrc);

        Server  &operator=(Server const & rRhs);

};

#endif
