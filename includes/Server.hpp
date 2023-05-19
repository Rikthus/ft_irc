#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>

class Server
{
    public:

        Server(char *port, char *password);
        ~Server(void);

        Server  &operator=(Server const & rRhs);

    private:

        Server(void);
        Server(Server const &rSrc);

};

#endif
