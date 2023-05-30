/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:22:10 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/30 18:29:01 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Core/ft_irc.hpp"

class Bot
{
	public:
		Bot(std::string name);
		~Bot();
		
		std::string	getName() {return name;}
		int			getFd() {return mSockfd;}
	private:
		std::string			name;
		int					mSockfd;
		fd_set				readfds;
		struct	sockaddr_in	mBotAddr;
};