/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:05 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/31 17:18:45 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/Commands.hpp"

void	PASS::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
{
	if (args.size() != 2)
	{
		send(iterator->first, "Invalid use of PASS\r\n", 22, 0);
		return ;
	}
	std::string	password = args[1];
	if (iterator->second.getAuthentication() == true)
	{
		send(iterator->first, "You're already authenticated\r\n", 31, 0);
		return ;
	}
	if (password == server->getPwd())
		iterator->second.setAuthentication();
	else if (password == server->getSpecialPwd())
	{
		iterator->second.setAuthentication();
		iterator->second.setBotBool();
	}
	else
		send(iterator->first, ":irc.project.com 464 :Incorrect password\r\n", 43, 0);

}

PASS::PASS()
{}

PASS::~PASS()
{}