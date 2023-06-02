/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:25 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/02 13:03:17 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/Commands.hpp"

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
	for (std::map<std::string, Channel>::iterator it1 = server->getChannelList().begin(); it1 != server->getChannelList().end(); it1++)
	{
		for (std::map<int, Client*>::iterator it2 = it1->second.getClientList().begin(); it2 != it1->second.getClientList().end(); it2++)
		{
			if (iterator->second.getNickname() == it2->second->getNickname())
			{
				it1->second.getClientList().erase(it2);
				return ;
			}
		}
	}
}

QUIT::QUIT()
{}

QUIT::~QUIT()
{}
