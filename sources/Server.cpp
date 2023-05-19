#include "../includes/Server.hpp"

//**********************************//
//              PUBLIC              //
//**********************************//

//////////////////////////
//      BUILDERS        //
//////////////////////////

Server::Server(char *port, char *pwd)
{
	mServerPort = std::atoi(port);
	mPwd = pwd;
}

Server::~Server(void)	{}


//**********************************//
//              PRIVATE             //
//**********************************//

//////////////////////////
//		BUILDERS		//
//////////////////////////

Server::Server(Server const &rSrc)	{(void)rSrc;}
Server::Server(void)    {}

//////////////////////////////////
//      OPERATORS OVERLOAD      //
//////////////////////////////////

Server	&Server::operator=(Server const &rRhs)	{(void)rRhs; return (*this);}
