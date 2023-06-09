/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 17:35:29 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/07 16:16:23 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/Commands.hpp"

int		PRIVMSG::findUser(std::map<int,Client> mClientList, std::string name)
{
	for (std::map<int,Client>::iterator it = mClientList.begin(); it != mClientList.end(); it++)
	{
		if (it->second.getNickname() == name && it->second.getRegistration())
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

bool				PRIVMSG::checkExistingClient(std::map<int, Client> mClientList, std::string name)
{
	for (clientIt it = mClientList.begin(); it != mClientList.end(); it++)
		if (it->second.getNickname() == name)
			return true;
	return false;
}

bool				PRIVMSG::checkExistingChannel(std::map<std::string, Channel> mChannelList, std::string name)
{
	for (channelIt it = mChannelList.begin(); it != mChannelList.end(); it++)
		if (it->second.getName() == name)
			return true;
	return false;
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

bool	clientIsInChan(std::string clientName, Channel chan)
{
	std::map<int, Client *> clientList = chan.getClientList();
	for (std::map<int, Client *>::iterator it = clientList.begin(); it != clientList.end(); it++)
	{
		if (it->second->getNickname() == clientName)
			return true;
	}
	return false;
}

void			PRIVMSG::sendMessageInChannel(std::string chanName, std::map<std::string, Channel> chanList, std::string message, clientIt iterator)
{
	for (channelIt it = chanList.begin(); it != chanList.end(); it++)
	{
		if (it->second.getName() == chanName)
		{
			if (clientIsInChan(iterator->second.getNickname(), it->second))
			{
				for (std::map<int, Client *>::iterator it2 = it->second.getClientList().begin(); it2 != it->second.getClientList().end(); it2++)
					send(it2->first, message.c_str(), message.size(), 0);
			}
			else
			{
				std::cout << "Didn't join channel here\n";
				return Rep().E442(iterator->first, iterator->second.getNickname(), chanName);
			}
		}
	}
}

void			PRIVMSG::sendMessageToUser(std::string nick, std::map<int, Client> mClientList, std::string message)
{
	for (clientIt it = mClientList.begin(); it != mClientList.end(); it++)
	{
		if (it->second.getNickname() == nick)
		{
			send(it->first, message.c_str(), message.size(), 0);
			return ;
		}
	}
}

std::string		buildMessage(std::vector<std::string> args, std::string name, std::string sender)
{
	std::string	ircMessage;
	
	for (unsigned int i = 2; i < args.size(); i++)
		ircMessage.append(args[i] += ' ');
	ircMessage.erase(ircMessage.size() - 1);
	std::string result = ":" + sender +  " PRIVMSG " + name + " " + ircMessage + "\r\n";
	return result;
}

void			PRIVMSG::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
{
	if (!iterator->second.getRegistration())
		return Rep().E451(iterator->first, iterator->second.getNickname());
	if (args.size() < 3)
		return Rep().E412(iterator->first, iterator->second.getNickname());

	std::vector<int>			clientList;
	std::vector<std::string>	names;
	std::string					line;
	std::string					message;
	int							fd = 0;
	
	if (args[1].find(',') != std::string::npos)
	{
		std::istringstream iss(args[1]);
		while (std::getline(iss, line, ','))
		{
			if (!checkExistingChannel(server->getChannelList(), line) && line[0] == '#')
				Rep().E403(iterator->first, iterator->second.getNickname(), line);
			else if (!checkExistingClient(server->getClientList(), line) && line[0] != '#')
				Rep().E401(iterator->first, iterator->second.getNickname(), line);
			else
				names.push_back(line);
		}
	}
	else if (args[1][0] == '#')
		clientList = findChannel(server->getChannelList(), args[1]);
	else
		fd = findUser(server->getClientList(), args[1]);
	if (!names.empty())
	{
		for (std::vector<std::string>::iterator it = names.begin(); it != names.end(); it++)
		{
			std::string	arg = *it;
			message = buildMessage(args, arg, iterator->second.getNickname());
			std::cout << "message sent: " << message;
			std::map<std::string, Channel>	chanList = server->getChannelList();
			if (arg[0] == '#')
				sendMessageInChannel(arg, server->getChannelList(), message, iterator);
			else
				sendMessageToUser(arg, server->getClientList(), message);
		}
	}
	else if (fd > 0)
	{
		message = buildMessage(args, args[1], iterator->second.getNickname());
		send(fd, message.c_str(), message.size(), 0);
	}
	else
	{
		message = buildMessage(args, args[1], iterator->second.getNickname());
		if (clientList.empty())
			return Rep().E403(iterator->first, iterator->second.getNickname(), args[1]);
		for (std::vector<int>::iterator it = clientList.begin(); it != clientList.end(); it++)
		{
			if (*it != iterator->first)
				send(*it, message.c_str(), message.size(), 0);
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
