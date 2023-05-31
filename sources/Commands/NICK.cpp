/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:43:38 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/31 17:11:48 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/Commands.hpp"

bool	NICK::checkDuplicateNick(std::string nickname, std::map<int,Client> mClientList)
{
	for (std::map<int,Client>::iterator it = mClientList.begin(); it != mClientList.end(); it++)
	{
		if (!it->second.getNickname().empty() && it->second.getNickname() == nickname)
			return true;
	}
	return false;
}

void	NICK::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
{
	if (!iterator->second.getAuthentication())
	{
		//send(iterator->first, "You need to enter the password before trying to register\n", 58, 0);
		return ;
	}
	if (args.size() != 2)
	{
		send(iterator->first, "Invalid use of NICK\n", 21, 0);
		return ;
	}
	std::string name = args[1];
	if (checkCharactersValidity(name) && !iterator->second.getIsBot())
	{
		std::string	error = ":irc.project.com 432 :Erroneous ";
		error.append(name += '\n');
		send(iterator->first, error.c_str(), error.size(), 0);
		return ;
	}
	if (!checkDuplicateNick(name, server->getClientList()))
		iterator->second.setNickname(name);
	else
	{
		std::string	error("Nickname already taken (" + name + ")\r\n");
		send(iterator->first, error.c_str(), error.size(), 0);
	}
}

NICK::NICK()
{}

NICK::~NICK()
{}