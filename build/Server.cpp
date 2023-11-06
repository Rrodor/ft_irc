/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:24:05 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/06 23:07:31 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

Server::Server(const char * port) : current_size(0), _port(std::atoi(port))
{
	this->_initServer();

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

/*void	Server::newUser(int & fd)
{
	int rc;
	int i = 5;
	char buffer[BUFFSIZE + 1];

	User *newuser = new User(fd);
	rc = read(fd, buffer, BUFFSIZE);
	rc = read(fd, buffer, BUFFSIZE);
	buffer[rc] = '\0';
	//std::cout << "nick : " << buffer << std::endl;
	newuser->nickname = buffer + 5;
	rc = read(fd, buffer, BUFFSIZE);
	buffer[rc] = '\0';
	//std::cout << "user : " << buffer << std::endl;
	while (buffer[i] != ' ')
		i++;
	buffer[i] = '\0';
	newuser->username = strdup(buffer + 5);
	newuser->realname = strdup(buffer + i + 4);
	this->users.push_back(newuser);
	std::cout << "new user connected, nickname :" << newuser->nickname;
	std::cout << " username :" << newuser->username;
	std::cout << " realname :" << newuser->realname << std::endl;

	std::string rpl_welcome = ":ft_irc 001 " + newuser->nickname + " :Welcome to the ft_irc Network, ";
	rpl_welcome += newuser->nickname + "\r\n";
	//rpl_welcome += newuser->nickname + "!" + newuser->username + "@" + newuser->realname + "\r\n";

	std::string rpl_yourhost = ":ft_irc 001 " + newuser->nickname + " :Your host is ft_irc, running version 1.0\r\n";

	std::string rpl_created = ":ft_irc 001 " + newuser->nickname + " :This server was created 2023/11/04 19:37:27\r\n";

	std::string rpl_myinfo = ":ft_irc 001 " + newuser->nickname + " :ft_irc 1.0 aoOirw abeiIklmnoOpqrstv\r\n";

	std::string rpl_msgoftheday = ":ft_irc 001 " + newuser->nickname + " :- ft_irc Message of the day - \r\n";
	send(fd, rpl_welcome.c_str() , 18, 0);
	send(fd, rpl_yourhost.c_str() , 18, 0);
	send(fd, rpl_created.c_str() , 18, 0);
	send(fd, rpl_myinfo.c_str() , 18, 0);
	send(fd, rpl_msgoftheday.c_str() , 18, 0);
}*/

void	Server::newUser(int & fd)
{
	int rc;
	int i = 5;
	char buffer[BUFFSIZE + 1];
	User *newuser = new User(fd);

	rc = read(fd, buffer, BUFFSIZE);
	buffer[rc] = '\0';
	std::cout << "buffer : " << buffer << std::endl;
	if (strncmp(buffer, "CAP", 3) == 0)
	{
		std::cout << "CAP" << std::endl;
		rc = read(fd, buffer, BUFFSIZE);
		buffer[rc] = '\0';
	}
	if (strncmp(buffer, "NICK", 4) == 0)
	{
		std::cout << "NICK" << std::endl;
		newuser->nickname = buffer + 5;
		rc = read(fd, buffer, BUFFSIZE);
		buffer[rc] = '\0';
	}
	if (strncmp(buffer, "USER", 4) == 0)
	{
		std::cout << "USER" << std::endl;
		while (buffer[i] != ' ')
			i++;
		buffer[i] = '\0';
		newuser->username = strdup(buffer + 5);
		newuser->realname = strdup(buffer + i + 4);
	}
	this->users.push_back(newuser);
	std::cout << "new user connected, nickname :" << newuser->nickname;
	//current_size++;

	fds.push_back(pollfd());
	this->fds[current_size].fd = fd;
	this->fds[current_size].events = POLLIN;
}

Server::~Server()
{
	shutdown(this->_serverSocket, SHUT_RDWR);
}
