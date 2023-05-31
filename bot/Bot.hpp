/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:22:10 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/31 13:46:11 by eavilov          ###   ########.fr       */
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
		std::vector<std::string> getFilter() {return badWords;}
	private:
		std::vector<std::string>	badWords;
		std::string					name;
		int							mSockfd;
		int							mOptval;
		fd_set						readfds;
		struct	sockaddr_in			mBotAddr;
};