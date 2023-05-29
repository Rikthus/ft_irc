/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:35:29 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/29 13:38:18 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/JOIN.hpp"

int		PRIVMSG::findUser(std::map<int,Client> clientList, std::string name)
{
	for (std::map<int,Client>::iterator it = clientList.begin(); it != clientList.end(); it++)
	{
		if (it->second.getNickname() == name)
			return it->first;
	}
	return -1;
}

void	PRIVMSG::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
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
	std::string ircMessage = "PRIVMSG " + args[1] + " :" + message + "\r\n";
	std::string	fullMessage = ":" + iterator->second.getNickname() + " " + ircMessage;
	send(fd, fullMessage.c_str(), fullMessage.size(), 0);
	std::cout << iterator->second.getNickname() << " sent a private message to " << args[1] << std::endl;
}

PRIVMSG::PRIVMSG()
{}

PRIVMSG::~PRIVMSG()
{}