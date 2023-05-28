/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:45:56 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 16:27:13 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "PASS.hpp"

class NICK
{
	public:
		NICK(std::string arguments, Client &user, int fd, std::map<int,Client> mClientList);
		~NICK();
		bool	checkDuplicateNick(std::string nickname, std::map<int,Client> mClientList);
	private:
		const std::string type;
};

bool	checkCharacter(char character);
bool	checkCharactersValidity(std::string name);