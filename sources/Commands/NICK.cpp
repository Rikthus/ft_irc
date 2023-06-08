/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:43:38 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/05 14:04:56 by eavilov          ###   ########.fr       */
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
		return ;
	if (args.size() != 2)
		return Rep().E431(iterator->first, iterator->second.getNickname());
	std::string name = args[1];
	if (checkCharactersValidity(name) && !iterator->second.getIsBot())
		return Rep().E432(iterator->first, iterator->second.getNickname(), name);
	if (!checkDuplicateNick(name, server->getClientList()) && !iterator->second.getNickname().empty())
	{
		std::string	message = ":" + iterator->second.getNickname() + "!" + iterator->second.getUsername() + "@irc.project.com NICK :" + name + "\r\n";
		std::cout << "Changing name\n";
		send(iterator->first, message.c_str(), message.size(), 0);
		iterator->second.setNickname(name);
	}
	else if (!checkDuplicateNick(name, server->getClientList()))
		iterator->second.setNickname(name);
	else
	{
		std::cout << "Already taken (" << args[1] << ")\n";
		return Rep().E433(iterator->first, iterator->second.getNickname(), name);
	}
}

NICK::NICK()
{}

NICK::~NICK()
{}