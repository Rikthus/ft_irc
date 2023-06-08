/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   botMain.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:33:18 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/08 18:15:55 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int	botSignal;

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
	else if (bufCpy.find("[FROM]_[server]_SHUTDOWN") != std::string::npos)
	{
		std::cerr << "Lost connection to server, shut me down pls" << std::endl;
		return ;
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
		if (read(fd, NULL, 0) > -1)
			send(fd, message.c_str(), message.size(), SOCK_STREAM);
	}
	else if (hasInsulted == true)
	{
		message.append("->1\r\n");
		if (read(fd, NULL, 0) > -1)
			send(fd, message.c_str(), message.size(), SOCK_STREAM);
	}
}

int main(int ac, char **av)
{
	botSignal = 0;
	if (ac != 2)
	{
		std::cout << "Error: correct usage -> ./Mildred <port>" << std::endl;
		return 1;
	}
	std::string	port = av[1];
	try
	{
		Bot	ElBoto("[Mildred]", port);
		send(ElBoto.getFd(), "PASS 27081998\r\nNICK [Mildred]\r\nUSER [Mildred] 0 * :[Mildred]\r\n", 63, SOCK_STREAM);
		std::cout << ElBoto.getGoose();
		while (!botSignal)
		{
			if (sigaction(SIGINT, &ElBoto.getSignalStruct(), 0) == -1 || botSignal)
				{return 1;}
				char	buffer[1024];
				int		endl = recv(ElBoto.getFd(), buffer, 1024, 0);
				buffer[endl] = 0;
				std::string	buf = buffer;
				parse(buf, ElBoto.getFd(), ElBoto.getFilter());
		}
	}
	catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
