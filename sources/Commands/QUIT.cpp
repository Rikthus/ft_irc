/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxperei <maxperei@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:25 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/03 16:09:50 by maxperei         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/Commands.hpp"

void	QUIT::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
{
	std::cout << "Client " << ntohs(iterator->second.getPort()) << " disconnected" << std::endl;
	std::string	message;
	std::string	protocol = ":" + iterator->second.getNickname() + "!" + iterator->second.getUsername() + "@irc.project.com ";
	std::string	nickname = iterator->second.getNickname();
	for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); it++)
		message.append(*it += " ");
	message.erase(message.size() - 1);
	message.erase(0, 5);
	if (message.empty())
	{
		message.erase();
		message = protocol + "QUIT :" + nickname + " has disconnected\r\n";
	}
	else
	{
		std::string	tmpMessage = protocol + "QUIT :" + nickname + " has disconnected (" + message + ")\r\n";
		message = tmpMessage;
	}
	close(iterator->first);
	FD_CLR(iterator->first, &server->getReadFds());
	server->getClientList().erase(iterator);
	if (!iterator->second.getNickname().empty())
		for (std::map<int,Client>::iterator it = server->getClientList().begin(); it != server->getClientList().end(); it++)
			send(it->first, message.c_str(), message.size(), 0);
	for (std::map<std::string, Channel>::iterator it1 = server->getChannelList().begin(); it1 != server->getChannelList().end(); it1++)
	{
		for (std::map<int, Client*>::iterator it2 = it1->second.getClientList().begin(); it2 != it1->second.getClientList().end(); it2++)
		{
			if (iterator->second.getNickname() == it2->second->getNickname())
			{
				it1->second.getClientList().erase(it2);
				for (std::vector<int>::iterator it3 = it1->second.getOperatorsList().begin(); it3 != it1->second.getOperatorsList().end(); it3++)
				{
					if (iterator->first == *it3)
						{it1->second.getOperatorsList().erase(it3);break;}
				}
				for (std::vector<int>::iterator it4 = it1->second.getInvitedList().begin(); it4 != it1->second.getInvitedList().end(); it4++)
				{
					if (iterator->first == *it4)
						{it1->second.getInvitedList().erase(it4);break;}
				}
				if (it1->second.getClientList().empty())
					server->getChannelList().erase(it1);
				return ;
			}
		}
	}
}

QUIT::QUIT()
{}

QUIT::~QUIT()
{}
