#include "../includes/Client.hpp"

//////************************************//////
//////				PUBLIC				  //////
//////************************************//////

//////////////////////////
//		BUILDERS		//
//////////////////////////

Client::Client(int socketFd, std::string nickname): mClientSocketFd(socketFd), mNickname(nickname) {}
Client::~Client(void)
{
	if (mClientSocketFd != -1)
		close(mClientSocketFd);
}

//////************************************//////
//////				PRIVATE				  //////
//////************************************//////

//////////////////////////
//		BUILDERS		//
//////////////////////////

Client::Client(Client const &rSrc)	{(void)rSrc;}
Client::Client(void)	{}

//////////////////////////////////
//		OPERATORS OVERLOAD		//
//////////////////////////////////

Client	&Client::operator=(Client const &rRhs)	{(void)rRhs; return (*this);}
