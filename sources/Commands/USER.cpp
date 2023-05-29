/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:30 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/29 18:08:13 by eavilov          ###   ########.fr       */
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
	if (!iterator->second.getAuthentication())
	{
		//send(iterator->first, "You need to enter the password before trying to register\n", 58, 0);
		return ;
	}
	if (args.size() != 5)
	{
		send(iterator->first, "Invalid use of USER\n", 21, 0);
		return ;
	}
	std::string	username = args[1];
	if (checkCharactersValidity(username))
	{
		std::string	error = ":irc.project.com 432 :Erroneous ";
		error.append(username += '\n');
		send(iterator->first, error.c_str(), error.size(), 0);
		return ;
	}
	if (!checkDuplicateUser(username, server->getClientList()))
	{
		iterator->second.setUsername(username);
		std::cout << username << " successfully registered." << std::endl;
		std::string	confirmation = ":irc.project.com 001 " + username + " :Welcome to the Internet Relay Network!\r\n";
		send(iterator->first, confirmation.c_str(), confirmation.size(), 0);
	}
	else
	{
		std::string	error("Username already taken (" + username + ")\r\n");
		send(iterator->first, error.c_str(), error.size(), 0);
	}
}

USER::USER()
{}

USER::~USER()
{}