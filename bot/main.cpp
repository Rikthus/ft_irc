/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/30 18:33:18 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/30 19:48:34 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

int main()
{
	Bot	ElBoto("Mildred");

	send(ElBoto.getFd(), "PASS 3333\r\n", 12, SOCK_STREAM);
	send(ElBoto.getFd(), "NICK Mildred\r\n", 15, SOCK_STREAM);
	send(ElBoto.getFd(), "USER Mildred 0 * :Mildred\r\n", 28, SOCK_STREAM);
	while (1)
	{
		char	buffer[1024];
		int		endl = recv(ElBoto.getFd(), buffer, sizeof(buffer), 0);
		buffer[endl] = 0;
		if (endl != 0)
			dprintf(1, "%s\n", buffer);
	}
}
