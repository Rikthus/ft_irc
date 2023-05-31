/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:06:26 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/31 17:08:37 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Core/Client.hpp"

Client::Client() : isAuthenticated(0), isRegistered(0), warns(0), isBot(false)
{
	clilen = sizeof(this->ClientAddr);
}

Client::~Client() {}

void Client::appendToBuffer(const char *data, int size)
{buffer.append(data, size);}

bool Client::extractMessageFromBuffer(std::string &message)
{
	size_t newlinePos = buffer.find('\n');
    if (newlinePos != std::string::npos)
    {
        message = buffer.substr(0, newlinePos);
        buffer.erase(0, newlinePos + 1);
        return true;
    }
    return false;
}
