/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:22:10 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/03 19:07:26 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Core/ft_irc.hpp"

extern int botSignal;

class Bot
{
	public:
		Bot(std::string name, std::string port); 
		Bot();
		~Bot();
		
		std::string	getName() {return name;}
		std::string	getGoose() {return goose;}
		int			getFd() {return mSockfd;}
		struct sigaction		&getSignalStruct() {return signalHandler;}
		std::vector<std::string> getFilter() {return badWords;}
		void		gooseBuilder();

	private:
		struct sigaction            signalHandler;
		std::vector<std::string>	badWords;
		std::string					name;
		std::string					goose;
		int							mSockfd;
		int							mOptval;
		fd_set						readfds;
		struct	sockaddr_in			mBotAddr;
};
