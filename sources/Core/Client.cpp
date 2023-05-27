/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tulipe <tulipe@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 11:06:26 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 23:16:24 by tulipe           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Core/Client.hpp"

Client::Client() : isAuthenticated(0), isRegistered(0)
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
