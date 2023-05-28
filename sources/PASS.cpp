/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:05 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 15:44:21 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/PASS.hpp"

PASS::PASS(std::string arguments, Client &user, std::string mPwd, int fd) : type("PASS")
{
	std::string password = arguments.substr(arguments.find(' ') + 1, arguments.size());
	if (password.find('\r') != std::string::npos)
		password.erase(password.find('\r'), std::string::npos);
	if (user.getAuthentication() == true)
	{
		return ;
	}
	if (password == mPwd)
	{
		user.setAuthentication();
	}
	else
		send(fd, "Incorrect password\n", 20, 0);
}

PASS::~PASS()
{}