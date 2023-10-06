/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:57:48 by babreton          #+#    #+#             */
/*   Updated: 2023/10/06 14:30:55 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <netinet/in.h>

typedef	int	SOCKET;

class Server {
	public:
		Server(const char * port);
		~Server();

		SOCKET				_newEntry;
	private: 
		Server(Server const &src);
		Server &	operator=(Server const &rSym);
		
		void	_initServer();

		SOCKET				_server;
		struct sockaddr_in	_adress;
		const int			_port;
};

#endif
