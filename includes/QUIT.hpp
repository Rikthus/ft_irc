#pragma once

#include "Client.hpp"

class QUIT
{
	public:
		QUIT(std::string arguments, std::map<int,Client> ClientList, std::string username);
		~QUIT();
		void sendQuitMessage();

	private:
		const		std::string type;
		std::map<int,Client> mClientList;
		std::string	quitMessage;
};
