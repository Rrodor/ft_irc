/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:57:48 by babreton          #+#    #+#             */
/*   Updated: 2023/10/14 19:00:00 by rrodor           ###   ########.fr       */
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
		int 		getServer() const;
	private:

		void	_initServer();

		int				_server;
		struct sockaddr_in	_adress;
		const int			_port;
};

#endif
