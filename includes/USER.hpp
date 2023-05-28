/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 12:44:57 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 15:59:21 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "NICK.hpp"

class USER
{
	public:
		USER(std::string arguments, Client &user, int fd, std::map<int,Client> mClientList);
		~USER();
		bool	checkDuplicateUser(std::string nickname, std::map<int,Client> mClientList);
	private:
		const std::string type;
};