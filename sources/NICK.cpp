/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:43:38 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 16:26:36 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/NICK.hpp"

bool	NICK::checkDuplicateNick(std::string nickname, std::map<int,Client> mClientList)
{
	for (std::map<int,Client>::iterator it = mClientList.begin(); it != mClientList.end(); it++)
	{
		if (!it->second.getNickname().empty() && it->second.getNickname() == nickname)
			return true;
	}
	return false;
}

NICK::NICK(std::string arguments, Client &user, int fd, std::map<int,Client> mClientList) : type("NICK")
{
	std::string name = arguments.substr(arguments.find(' ') + 1, arguments.size());

	if (name.find('\r') != std::string::npos)
		name.erase(name.find('\r'), std::string::npos);
	if (checkCharactersValidity(name))
	{
		send(fd, "Invalid name format\n", 21, 0);
		return ;
	}
	if (!checkDuplicateNick(name, mClientList))
	{
		user.setNickname(name);
	}
	else
		send(fd, "Nickname already taken\n", 24, 0);
}

NICK::~NICK()
{}