/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:24:05 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/11 18:47:53 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

Server::Server(const char * port, std::string password) : current_size(0), _port(std::atoi(port))
{
	this->_initServer();
	this->password = password;

	/* Init Channels //
	//this->_general = new Channel("general", "general", 0);
	//this->_channels.insert(std::make_pair(this->_general->getName(), this->_general));
	std::cout << "Channels list size : " << this->getChannelsListSize() << std::endl;*/

	fds.push_back(pollfd());
	this->fds[0].fd = this->_serverSocket;
	this->fds[0].events = POLLIN;
}

int		Server::getServerSocket() const
{
	return this->_serverSocket;
}

void	Server::_initServer() {
	int	opt = 1;
	int	addrlen = sizeof(this->_adress);

	if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw std::runtime_error("[ERROR] : can't create a socket, please check your permissions or cpu arch.");

	if (setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw std::runtime_error("[ERROR] : can't set options of socket, please check your permissions or cpu arch.");

	if (ioctl(this->_serverSocket, FIONBIO, (char *)&opt))
		throw std::runtime_error("[ERROR] : can't recover device information, please check your permissions or cpu arch.");

	memset((char *)&this->_adress, 0, sizeof(this->_adress));
	this->_adress.sin_family = AF_INET;
	this->_adress.sin_addr.s_addr = inet_addr("127.0.0.1");
	this->_adress.sin_port = htons(this->_port);

	if (bind(this->_serverSocket, (struct sockaddr*)&this->_adress, sizeof(this->_adress)) < 0)
		throw std::runtime_error("[ERROR] : can't get local adress informations, bind failed, please check your permissions or cpu arch.");

	if (listen(this->_serverSocket, 3) < 0)
		throw std::runtime_error("[ERROR] : can't listen on the adress, please check your permissions or cpu arch.");
}

void	Server::newUser(int & fd)
{
	int rc;
	int i = 5;
	char buffer[BUFFSIZE + 1];
	User *newuser = new User(fd);

	rc = read(fd, buffer, BUFFSIZE);
	buffer[rc] = '\0';
	read_log(fd, buffer, this);
	if (strncmp(buffer, "PASS", 4) == 0)
	{
		buffer[rc - 2] = '\0';
		std::string pass = buffer + 5;
		if (pass != this->password)
		{
			std::string message = ERR_PASSWDMISMATCH;
			send(fd, message.c_str(), message.length(), 0);
			send_log(fd, message.c_str(), this);
			throw (WrongPasswordException());
		}
		rc = read(fd, buffer, BUFFSIZE);
		buffer[rc] = '\0';
		read_log(fd, buffer, this);
	}
	else
	{
		std::string message = ERR_PASSWDMISMATCH;
		send(fd, message.c_str(), message.length(), 0);
		send_log(fd, message.c_str(), this);
		throw (WrongPasswordException());
	}
	if (strncmp(buffer, "CAP", 3) == 0)
	{
		rc = read(fd, buffer, BUFFSIZE);
		buffer[rc] = '\0';
		read_log(fd, buffer, this);
	}
	if (strncmp(buffer, "NICK", 4) == 0)
	{
		buffer[rc - 2] = '\0';
		newuser->nickname = strdup(buffer + 5);
		rc = read(fd, buffer, BUFFSIZE);
		buffer[rc] = '\0';
		read_log(fd, buffer, this);
	}
	if (strncmp(buffer, "USER", 4) == 0)
	{
		while (buffer[i] != ' ')
			i++;
		buffer[i] = '\0';
		newuser->username = strdup(buffer + 5);
		newuser->realname = strdup(buffer + i + 4);
	}
	this->users.push_back(newuser);
	//current_size++;

	fds.push_back(pollfd());
	this->fds[current_size].fd = fd;
	this->fds[current_size].events = POLLIN;

	std::string message = ":127.0.0.1 001 " + newuser->nickname + " :Welcome to the ft_irc network, " + newuser->nickname + "\r\n";
	send(fd, message.c_str(), message.length(), 0);
	send_log(fd, message.c_str(), this);
	std::cout << YELLOW << "New user " << newuser->nickname << " succesfully registered with id " << fd << "." << RESET << std::endl;
}

Server::~Server()
{
	shutdown(this->_serverSocket, SHUT_RDWR);
}

const char* Server::WrongPasswordException::what() const throw()
{
	return "Wrong password.";
}
