/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:05 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/02 17:09:46 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/Commands.hpp"

void	PASS::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
{
	if (args.size() != 2)
		return Rep().E464(iterator->first, iterator->second.getNickname());
	std::string	password = args[1];
	if (iterator->second.getAuthentication() == true)
		return Rep().E462(iterator->first, iterator->second.getNickname());
	if (password == server->getPwd())
		iterator->second.setAuthentication();
	else if (password == server->getSpecialPwd())
	{
		iterator->second.setAuthentication();
		iterator->second.setBotBool();
	}
	else
		return Rep().E464(iterator->first, iterator->second.getNickname());

}

PASS::PASS()
{}

PASS::~PASS()
{}