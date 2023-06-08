/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:06:26 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/08 16:45:12 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Core/Client.hpp"

Client::Client() : isAuthenticated(0), isRegistered(0), warns(0), isBot(false)
{
    hasNickname = 0;
	clilen = sizeof(this->ClientAddr);
}

Client::~Client() {}

void Client::appendToBuffer(char * data, int size)
{
    if (data)
    {
        if ((size_t)size > std::strlen(data))
            return ;
        buffer.append(data, size);
    }
}

bool Client::extractMessageFromBuffer(std::string &message, int fd)
{
    if (read(fd, NULL, 0) < 0)
        return false;
    size_t newlinePos = buffer.find('\n');
    if (newlinePos != std::string::npos)
    {
        message = buffer.substr(0, newlinePos);
        buffer.erase(0, newlinePos + 1);
        return true;
    }
    return false;
}
