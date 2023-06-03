/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxperei <maxperei@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:35:29 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/03 19:01:24 by maxperei         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/Commands.hpp"

int		PRIVMSG::findUser(std::map<int,Client> mClientList, std::string name)
{
	for (std::map<int,Client>::iterator it = mClientList.begin(); it != mClientList.end(); it++)
	{
		if (it->second.getNickname() == name)
			return it->first;
	}
	return -1;
}

std::vector<int>	PRIVMSG::findChannel(std::map<std::string, Channel>	mChannelList, std::string name)
{
	std::vector<int>	fdList;
	for (std::map<std::string,Channel>::iterator it = mChannelList.begin(); it != mChannelList.end(); it++)
	{
		if (it->second.getName() == name)
		{
			std::map<int,Client *> clientList = it->second.getClientList();
			for (std::map<int, Client *>::iterator itt = clientList.begin(); itt != clientList.end(); itt++)
				fdList.push_back(itt->first);
			return fdList;
		}
	}
	return fdList;
}

std::string itoa(int a)
{
    std::string ss="";
    while(a)
    {
        int		x = a % 10;
        char	i = '0';
        a /= 10;
        i = i + x;
        ss = i + ss;
    }
    return ss;
}

void	PRIVMSG::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
{
	(void) server;
	if (!iterator->second.getRegistration())
		return Rep().E451(iterator->first, iterator->second.getNickname());
	if (args.size() < 3)
		return Rep().E412(iterator->first, iterator->second.getNickname());

	std::vector<int>	clientList;
	int			fd = 0;
	
	if (args[1][0] == '#')
		clientList = findChannel(server->getChannelList(), args[1]);
	else
		fd = findUser(server->getClientList(), args[1]);
	if (fd == -1)
		return Rep().E401(iterator->first, iterator->second.getNickname(), args[1]);
	std::string message;
	for (unsigned int i = 2; i < args.size(); i++)
		message.append(args[i] += ' ');
	message.erase(message.size() - 1);
	std::string ircMessage = "PRIVMSG " + args[1] + " " + message + "\r\n";
	std::string	fullMessage = ":" + iterator->second.getNickname() + " " + ircMessage;
	if (fd != 0)
		send(fd, fullMessage.c_str(), fullMessage.size(), 0);
	else
	{
		if (clientList.empty())
			return Rep().E403(iterator->first, iterator->second.getNickname(), args[1]);
		for (std::vector<int>::iterator it = clientList.begin(); it != clientList.end(); it++)
		{
			if (*it != iterator->first)
				send(*it, fullMessage.c_str(), fullMessage.size(), 0);
		}
	}
	std::cout << iterator->second.getNickname() << " sent a private message to " << args[1] << std::endl;
	std::string	messageToBot = "[FROM]_" + iterator->second.getNickname() + ": " + message + "\r\n";
	if (server->getBotFd() > 0)
		send(server->getBotFd(), messageToBot.c_str(), messageToBot.size(), SOCK_STREAM);
}

PRIVMSG::PRIVMSG()
{}

PRIVMSG::~PRIVMSG()
{}