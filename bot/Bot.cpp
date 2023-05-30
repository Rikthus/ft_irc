/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:21:49 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/30 18:32:26 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include "../includes/Core/Server.hpp"

Bot::Bot(std::string name) : name(name), mSockfd(socket(AF_INET, SOCK_STREAM, 0))
{
	std::cout << "Bot assembly engaged" << std::endl;
	if (mSockfd < 0)
    	throw(std::runtime_error("Error: socket attribution failed"));
	FD_ZERO(&readfds);FD_SET(mSockfd, &readfds);
	mBotAddr.sin_family = AF_INET;
	mBotAddr.sin_addr.s_addr = INADDR_ANY;
	mBotAddr.sin_port = htons(6667);
	if (connect(mSockfd, (struct sockaddr *)&mBotAddr, sizeof(mBotAddr)) < 0)
		throw(std::runtime_error("Error: connection to server failed"));
}

Bot::~Bot() {}