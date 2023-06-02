/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:21:49 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/02 13:48:06 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "../includes/Core/Server.hpp"

void	exitHandler(int signal)
{
	(void) signal;
	botSignal = 1;
}

Bot::Bot(std::string name, std::string port) : name(name), mSockfd(socket(AF_INET, SOCK_STREAM, 0)), mOptval(1)
{
	int mPort = 0; mPort=std::atoi(port.c_str());
	if (port.empty() || !mPort)
		throw(std::runtime_error("Error: no port given"));
	std::cout << "Bot assembly engaged" << std::endl;
	if (mSockfd < 0)
    	throw(std::runtime_error("Error: socket attribution failed"));
	if (setsockopt(mSockfd, SOL_SOCKET, SO_REUSEPORT, &mOptval, sizeof(mOptval)) < 0)
        throw(std::runtime_error("Error: socket option attribution failed"));
	FD_ZERO(&readfds);FD_SET(mSockfd, &readfds);
	mBotAddr.sin_family = AF_INET;
	mBotAddr.sin_addr.s_addr = INADDR_ANY;
	mBotAddr.sin_port = htons(mPort);
	if (connect(mSockfd, (struct sockaddr *)&mBotAddr, sizeof(mBotAddr)) < 0)
		throw(std::runtime_error("Error: connection to server failed"));
	signalHandler.sa_handler = exitHandler;
	sigemptyset(&signalHandler.sa_mask);
	signalHandler.sa_flags = 0;
	badWords.push_back("tg");
	badWords.push_back("fdp");
	badWords.push_back("pd");
	badWords.push_back("ntm");
	badWords.push_back("encule");
	badWords.push_back("putain");
	badWords.push_back("je joue range top");
	badWords.push_back("salope");
	badWords.push_back("pute");
}

Bot::Bot() {}

Bot::~Bot()
{
	std::cout << "Bot destructor called" << std::endl;
	if (mSockfd > 0)
		close (mSockfd);
}
