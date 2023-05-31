/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:33:18 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/31 17:32:41 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

void	parse(std::string buffer, int fd, std::vector<std::string> badWords)
{
	bool		hasInsulted = false;
	bool		hasSinned = false;
	std::string	bufCpy = buffer;
	std::string	name;
	std::string	clientMessage;
	std::string	message = "[FROM_BOT]_";
	if (bufCpy.find("[FROM]_") != std::string::npos)
	{
		bufCpy.erase(0, 7);
		name = bufCpy.substr(0, bufCpy.find(':'));
		clientMessage = bufCpy.substr(bufCpy.find(':') + 2, std::string::npos);
	}
	message.append(name);
	for (std::vector<std::string>::iterator it = badWords.begin(); it != badWords.end(); it++)
	{
		if (clientMessage.find(*it) != std::string::npos)
			hasInsulted = true;
		if (clientMessage.find(*it) != std::string::npos && *it == "je joue range top")
			hasSinned = true;
	}
	if (hasSinned == true)
	{
		message.append("->3\r\n");
		send(fd, message.c_str(), message.size(), SOCK_STREAM);
	}
	else if (hasInsulted == true)
	{
		message.append("->1\r\n");
		send(fd, message.c_str(), message.size(), SOCK_STREAM);
	}
}

int main()
{
	Bot	ElBoto("[Mildred]");

	send(ElBoto.getFd(), "PASS 2708\r\nNICK [Mildred]\r\nUSER [Mildred] 0 * :[Mildred]\r\n", 59, SOCK_STREAM);
	while (1)
	{
		char	buffer[1024];
		int		endl = recv(ElBoto.getFd(), buffer, 1024, 0);
		buffer[endl] = 0;
		std::string	buf = buffer;
		parse(buf, ElBoto.getFd(), ElBoto.getFilter());
	}
}
