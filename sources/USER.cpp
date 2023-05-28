/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:30 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/28 12:03:20 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/USER.hpp"

bool	USER::checkDuplicateUser(std::string username, std::map<int,Client> mClientList)
{
	for (std::map<int,Client>::iterator it = mClientList.begin(); it != mClientList.end(); it++)
	{
		if (!it->second.getUsername().empty() && it->second.getUsername() == username)
			return true;
	}
	return false;
}

USER::USER(std::string arguments, Client &user, int fd, std::map<int,Client> mClientList) : type("USER")
{
	std::string name = arguments.erase(0, arguments.find(' ') + 1);

	if (name.find(' ') != std::string::npos)
		name.erase(name.find(' '), std::string::npos);
	if (checkCharactersValidity(name))
	{
		send(fd, "Invalid name format", 21, 0);
		return ;
	}
	if (!checkDuplicateUser(name, mClientList))
	{
		user.setUsername(name);
	 	std::string	confirmation = ":irc.project.com 001 "; confirmation.append(name); confirmation.append(" :Welcome to the Internet Relay Network!\n");
		send(fd, confirmation.c_str(), confirmation.size(), 0);
	}
	else
		send(fd, "Username already taken", 23, 0);
}

USER::~USER()
{}