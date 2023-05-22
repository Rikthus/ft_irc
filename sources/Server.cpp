#include "../includes/Server.hpp"

//////**********************************//////
//////              PUBLIC              //////
//////**********************************//////

//////////////////////////
//      FUNCTIONS       //
//////////////////////////

void    Server::launch(void)
{
    
}

//////////////////////////
//      BUILDERS        //
//////////////////////////

Server::Server(char *port, char *pwd)
{
	if (this->portVerif(port))
	{
		mServerPort = std::atoi(port);
		mPwd = pwd;
	}
	else
		throw(std::runtime_error("Error: invalid port (6660-6669 or 7000)"));
}

Server::~Server(void)	{}


//////**********************************//////
//////              PRIVATE             //////
//////**********************************//////

//////////////////////////
//		FUNCTIONS		//
//////////////////////////

bool Server::portVerif(char *str)	const
{
    int value;

    for (int i = 0; str[i]; i++)
    {
        if (i > 3 || !std::isdigit(str[i]))
            return (false);
    }
    value = std::atoi(str);
    if ((value < 6660 || value > 6669) && value != 7000)
        return (false);
    return (true);
}

//////////////////////////
//		BUILDERS		//
//////////////////////////

Server::Server(Server const &rSrc)	{(void)rSrc;}
Server::Server(void)    {}

//////////////////////////////////
//      OPERATORS OVERLOAD      //
//////////////////////////////////

Server	&Server::operator=(Server const &rRhs)	{(void)rRhs; return (*this);}
