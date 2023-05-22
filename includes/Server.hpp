#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <sys/types.h>

class Server
{
    public:

        void    launch(void);

        Server(char *port, char *pwd);
        ~Server(void);

    private:

        std::string mPwd;
        int         mServerPort;

        bool portVerif(char *str) const;

        Server(void);
        Server(Server const &rSrc);
        Server  &operator=(Server const & rRhs);

};

#endif
