/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:05 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/28 16:44:15 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Commands/JOIN.hpp"

void	PASS::execute(Server *server, clientIt &iterator, std::vector<std::string> args)
{
	if (args.size() != 2)
	{
		send(iterator->first, "Invalid use of PASS\n", 21, 0);
		return ;
	}
	std::string	password = args[1];
	if (iterator->second.getAuthentication() == true)
		return ;
	if (password == server->getPwd())
		iterator->second.setAuthentication();
	else
		send(iterator->first, "Incorrect password\n", 20, 0);

}

PASS::PASS()
{}

PASS::~PASS()
{}