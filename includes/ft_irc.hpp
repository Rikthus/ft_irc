/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eavilov <eavilov@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 19:21:48 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 11:02:38 by eavilov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <map>

#define PORT 6667
#define PASSWORD "3333\0"

/* Internet Relay Chat :
Les clients IRC se connectent à des serveurs IRC afin d’accéder à des canaux.
Les serveurs IRC sont connectés entre eux afin de créer des réseaux.

weechat:
create server - /server add <NAME> 127.0.0.1/<PORT> -password=<PASSWORD>
connect to server - /connect <NAME>

https://ircgod.com/docs/irc/to_know/

*/
