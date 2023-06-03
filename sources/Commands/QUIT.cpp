/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxperei <maxperei@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:25 by eavilov           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/06/03 12:55:02 by maxperei         ###   ########lyon.fr   */
=======
/*   Updated: 2023/06/03 13:59:30 by eavilov          ###   ########.fr       */
>>>>>>> 22f603761da39c13f98eebdffa89c692d3ed597a
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
		message.erase();
		message = "QUIT :" + nickname + " has disconnected\r\n";
	}
	else
	{
		std::string	tmpMessage = "QUIT:" + nickname + " has disconnected (" + message + ")\r\n";
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
				return ;
			}
		}
	}
}

QUIT::QUIT()
{}

QUIT::~QUIT()
{}
