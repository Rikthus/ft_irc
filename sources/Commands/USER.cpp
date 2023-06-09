/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:30 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/05 13:57:45 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/Commands.hpp"

bool	USER::checkDuplicateUser(std::string username, std::map<int,Client> mClientList)
{
	for (std::map<int,Client>::iterator it = mClientList.begin(); it != mClientList.end(); it++)
	{
		if (!it->second.getUsername().empty() && it->second.getUsername() == username)
			return true;
	}
	return false;
}

void	USER::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
{
	if (!iterator->second.getAuthentication() || !iterator->second.hasNick())
		return ;
	if (args.size() != 5)
		return Rep().E461(iterator->first, iterator->second.getNickname(), args[0]);
	std::string	username = args[1];
	if (checkCharactersValidity(username) && !iterator->second.getIsBot())
		return Rep().E432(iterator->first, iterator->second.getNickname(), username);
	if (!checkDuplicateUser(username, server->getClientList()) && iterator->second.getUsername().empty())
	{
		iterator->second.setUsername(username);
		iterator->second.setRegistration();
		if (username == "[Mildred]")
			server->setBotFd(iterator->first);
		std::cout << iterator->second.getNickname() << " successfully registered." << std::endl;
		Rep().R001(iterator->first, iterator->second.getNickname(), server->getPigeon());
		Rep().R002(iterator->first, iterator->second.getNickname(), "*irc de la rue zebi 😂👌*", "0.42");
		Rep().R003(iterator->first, iterator->second.getNickname(), "on a monday");
		Rep().R004(iterator->first, iterator->second.getNickname());
		
	}
	else
		return Rep().E433(iterator->first, iterator->second.getNickname(), args[1]);
}

USER::USER()
{}

USER::~USER()
{}
