/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:05 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/05 13:41:13 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/Commands.hpp"

void	PASS::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
{
	if (args.size() != 2)
	{
		Rep().E464(iterator->first, iterator->second.getNickname());
		// close(iterator->first);
		// FD_CLR(iterator->first, &server->getReadFds());
		// server->getClientList().erase(iterator);
		return ;
	}
	std::string	password = args[1];
	if (iterator->second.getAuthentication() == true)
		return Rep().E462(iterator->first, iterator->second.getNickname());
	if (password == server->getPwd())
		iterator->second.setAuthentication();
	else if (password == server->getSpecialPwd())
	{
		iterator->second.setAuthentication();
		iterator->second.setBotBool();
	}
	else
	{
		Rep().E464(iterator->first, iterator->second.getNickname());
		// close(iterator->first);
		// FD_CLR(iterator->first, &server->getReadFds());
		// server->getClientList().erase(iterator);
		return ;
	}

}

PASS::PASS()
{}

PASS::~PASS()
{}
