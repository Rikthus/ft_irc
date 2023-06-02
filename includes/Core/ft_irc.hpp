/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 19:21:48 by eavilov           #+#    #+#             */
/*   Updated: 2023/06/02 14:29:32 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <stdlib.h>

# include <signal.h>
# include <iostream>
# include <string>
# include <sstream>
# include <cstring>
# include <cstdio>
# include <algorithm>
# include <cstdlib>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <vector>
# include <map>

/* Internet Relay Chat :
Les clients IRC se connectent à des serveurs IRC afin d’accéder à des canaux.
Les serveurs IRC sont connectés entre eux afin de créer des réseaux.

https://ircgod.com/docs/irc/to_know/

*/
