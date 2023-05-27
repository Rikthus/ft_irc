#include "../../includes/Commands/JOIN.hpp"

ACmd::ACmd(void) {}
ACmd::~ACmd(void) {}

void	JOIN::execute(Server *server, int clientSockfd, Client &clientData, std::string msg)
{
	//ADD Parsing
	(void)server;
	(void)clientSockfd;
	(void)clientData;
	(void)msg;
	std::cout << "JOINED" << std::endl;
	std::cout << msg << std::endl;
}

JOIN::JOIN(void) {}

JOIN::~JOIN(void) {}