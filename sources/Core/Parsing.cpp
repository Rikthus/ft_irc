/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/27 15:52:20 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 15:53:08 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

bool	checkCharacter(char character)
{
	std::string	validCharacters("aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ0123456789");
	
	if (validCharacters.find(character) != std::string::npos)
		return false;
	else
		return true;
}

bool	checkCharactersValidity(std::string name)
{
	for (size_t index = 0; index < name.size(); index++)
	{
		if (checkCharacter(name[index]))
			return true;
	}
	return false;
}
