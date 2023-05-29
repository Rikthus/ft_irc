/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 14:22:20 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/29 14:26:55 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/JOIN.hpp"

int		NOTICE::findUser(std::map<int,Client> clientList, std::string name)
{
	for (std::map<int,Client>::iterator it = clientList.begin(); it != clientList.end(); it++)
	{
		if (it->second.getNickname() == name)
			return it->first;
	}
	return -1;
}

void	NOTICE::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
{
	if (args.size() < 3)
	{
		send(iterator->first, "412 ERR_NOTEXTTOSEND :No text to send\r\n", 40, 0);
		return ;
	}
	int	fd = findUser(server->getClientList(), args[1]);
	if (fd < 0)
	{
		send(iterator->first, "401 ERR_NOSUCHNICK :No such nick/channel\r\n", 43, 0);
		return ;
	}
	std::string message;
	for (unsigned int i = 2; i < args.size(); i++)
		message.append(args[i] += ' ');
	message.erase(message.size() - 1);
	std::string ircMessage = "NOTICE " + args[1] + " :" + message + "\r\n";
	send(fd, ircMessage.c_str(), ircMessage.size(), 0);
	std::cout << iterator->second.getNickname() << " sent a notice to " << args[1] << std::endl;
}


NOTICE::NOTICE()
{}

NOTICE::~NOTICE()
{}