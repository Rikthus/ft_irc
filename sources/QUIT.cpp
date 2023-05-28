/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:25 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 15:23:34 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/QUIT.hpp"

QUIT::QUIT(std::string arguments, std::map<int,Client> ClientList, std::string username) : type("QUIT"), mClientList(ClientList)
{
	std::string msg = arguments;
	if (msg.find('\r') != std::string::npos)
		msg.erase(msg.find('\r'));
	if (msg.size() > 4 && msg.substr(0, 6) != "QUIT :")
		throw(std::runtime_error("Error: invalid use of QUIT"));
	msg.erase(0, 6);
	if (msg.empty())
	{
		this->quitMessage = username.append(" has disconnected from the server");
	}
	else
	{
		this->quitMessage = username.append(": ").append(msg);
	}
}

QUIT::~QUIT()
{}

void	QUIT::sendQuitMessage()
{
	std::string message = this->quitMessage;
	for (std::map<int,Client>::iterator it = mClientList.begin(); it != mClientList.end(); it++)
		send(it->first, message.append("\n").c_str(), message.size() + 1, 0);
}
