/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:25 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/29 14:46:55 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/JOIN.hpp"

void	QUIT::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
{
	std::cout << "Client " << ntohs(iterator->second.getPort()) << " disconnected" << std::endl;
	std::string	message;
	std::string	nickname = iterator->second.getNickname();
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
		message.append(*it += " ");
	message.erase(message.size() - 1);
	message.erase(0, 5);
	if (message == ":WeeChat 3.8" || message.empty())
	{
		std::cout << "Message is empty\n";
		message.erase();
		message = "QUIT :" + nickname + " has disconnected\r\n";
	}
	else
	{
		std::string	tmpMessage = "QUIT :" + nickname + " has disconnected (" + message + ")\r\n";
		message = tmpMessage;
	}
	close(iterator->first);
	FD_CLR(iterator->first, &server->getReadFds());
	server->getClientList().erase(iterator);
	if (!iterator->second.getNickname().empty())
		for (std::map<int,Client>::iterator it = server->getClientList().begin(); it != server->getClientList().end(); it++)
			send(it->first, nickname.c_str(), nickname.size(), 0);
}

QUIT::QUIT()
{}

QUIT::~QUIT()
{}
