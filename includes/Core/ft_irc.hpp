/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tulipe <tulipe@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 19:21:48 by eavilov           #+#    #+#             */
/*   Updated: 2023/05/27 23:30:41 by tulipe           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

# include <stdlib.h>

# include <iostream>
# include <string>
# include <sstream>
# include <cstring>
# include <cstdio>
# include <cstdlib>
# include <unistd.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <vector>
# include <map>

class Server;
class Client;
class Channel;
class ACmd;
class JOIN;

#endif

/* Internet Relay Chat :
Les clients IRC se connectent à des serveurs IRC afin d’accéder à des canaux.
Les serveurs IRC sont connectés entre eux afin de créer des réseaux.

https://ircgod.com/docs/irc/to_know/

*/
