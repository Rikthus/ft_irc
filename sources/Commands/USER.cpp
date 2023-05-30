/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:30 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/30 16:27:11 by eavilov          ###   ########.fr       */
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
		send(iterator->first, "Invalid use of USER\r\n", 22, 0);
		return ;
	}
	std::string	username = args[1];
	if (checkCharactersValidity(username))
	{
		std::string	error = ":irc.project.com 432 :Erroneous ";
		error.append(username += "\r\n");
		send(iterator->first, error.c_str(), error.size(), 0);
		return ; 
	}
	if (!checkDuplicateUser(username, server->getClientList()))
	{
		Rep	tmp;
		iterator->second.setUsername(username);
		std::cout << username << " successfully registered." << std::endl;
		Rep().R001(iterator->first, username);
		Rep().R002(iterator->first, username, "*irc de la rue zebi 😂👌*", "0.42");
		Rep().R003(iterator->first, username, "on a monday");
		Rep().R004(iterator->first, username);
		
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