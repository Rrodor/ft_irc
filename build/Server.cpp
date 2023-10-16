/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 13:35:22 by babreton          #+#    #+#             */
/*   Updated: 2023/10/14 19:09:32 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

Server::Server(const char * port) : _port(std::atoi(port))
{
	this->_initServer();
}

int		Server::getServer() const
{
	return (this->_server);
}

void	Server::_initServer() {
	int	opt = 1;
	int	addrlen = sizeof(this->_adress);

	if ((this->_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	if (setsockopt(this->_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	if (ioctl(this->_server, FIONBIO, (char *)&opt))
	{
		perror("ioctl() failed");
		exit(EXIT_FAILURE);
	}
	this->_adress.sin_family = AF_INET;
	this->_adress.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->_adress.sin_port = htons(this->_port);

	if (bind(this->_server, (struct sockaddr*)&this->_adress, sizeof(this->_adress)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(this->_server, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
}

Server::~Server()
{
	shutdown(this->_server, SHUT_RDWR);
}
