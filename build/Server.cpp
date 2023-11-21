/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:24:05 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/21 13:55:17 by babreton         ###   ########.fr       */
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

void	Server::_initServer()
{
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
	this->users.push_back(newuser);

	rc = read(fd, buffer, BUFFSIZE);
	buffer[rc] = '\0';
	read_log(fd, buffer, this);
	if (newuser->conStep == 1)
		this->passUser(fd, buffer, newuser);

	rc = read(fd, buffer, BUFFSIZE);
	buffer[rc] = '\0';
	read_log(fd, buffer, this);
	if (newuser->conStep == 2)
		this->capUser(fd, buffer, newuser);

	rc = read(fd, buffer, BUFFSIZE);
	buffer[rc] = '\0';
	read_log(fd, buffer, this);
	if (newuser->conStep == 3)
		this->nickUser(fd, buffer, newuser);

	rc = read(fd, buffer, BUFFSIZE);
	buffer[rc] = '\0';
	read_log(fd, buffer, this);
	if (newuser->conStep == 4)
		this->userUser(fd, buffer, newuser);
	
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

const char* Server::PassException::what() const throw()
{
	return "Error during PASS command";
}

const char* Server::CapException::what() const throw()
{
	return "Error during CAP command";
}

const char* Server::NickException::what() const throw()
{
	return "Error during NICK command";
}

const char* Server::UserException::what() const throw()
{
	return "Error during USER command";
}

void		Server::printServerChannels(std::string name) const
{
	std::string	clist = JOIN;
	clist += "[#" + name + "]" + "[CHANNELS_LIST] > ";
	for (std::vector<Channel *>::const_iterator it = this->channels.begin(); it != this->channels.end(); ++it)
	{
		clist += "|" + (*it)->name + "|";
	}
	std::cout << clist << std::endl;
}

std::vector<Channel *>::iterator	Server::getChannelByName(std::string name)
{
	if (name[0] == '#')
		name.erase(name.begin());
	std::vector<Channel *>::iterator	it = this->channels.begin();
	std::vector<Channel *>::iterator	ite = this->channels.end();

	if (it == ite)
		return this->channels.end();
	while (it != ite)
	{
		if ((*it)->name == name)
			return it;
		it++;
	}
	return this->channels.end();
}

std::vector<User *>::iterator	Server::getUserByName(std::string name)
{
	std::vector<User *>::iterator	it = this->users.begin();
	std::vector<User *>::iterator	ite = this->users.end();

	while (it != ite)
	{
		if ((*it)->nickname == name)
			return it;
		it++;
	}
	return this->users.end();
}

std::vector<User *>::iterator	Server::getUserByFd(int & fd)
{
	std::vector<User *>::iterator	it = this->users.begin();
	std::vector<User *>::iterator	ite = this->users.end();

	while (it != ite)
	{
		if ((*it)->fd == fd)
			return it;
		it++;
	}
	return this->users.end();
}

void	Server::checkChannel()
{
	std::vector<Channel *>::iterator	it = this->channels.begin();;

	while (it != this->channels.end())
	{
		if ((*it)->operators.empty() && !(*it)->users.empty())
			(*it)->allocNewOp(this);
		if ((*it)->operators.empty() && (*it)->users.empty())
		{
			std::cout << DELETE << "Channel " << (*it)->name << " is empty, erasing it from server..." << RESET << std::endl;
			it = this->channels.erase(it);
		}
		else
			it++;
	}
}

void	Server::deleteUser(int & fd)
{
	std::vector<User *>::iterator		user = this->getUserByFd(fd);
	std::vector<Channel *>::iterator	it = this->channels.begin();

	while (it != this->channels.end())
	{
		if ((*it)->isInChannel((*user)) || (*it)->isOpInChannel((*user)))
			(*it)->deleteChannelUser((*user), this, 1);
		it++;
	}
	delete (*user);
}

bool	Server::haveN(std::string message) const
{
	int	i = -1;

	while (message[++i])
	{
		if (message[i] == '\n')
			return true;
	}
	return false;
}

std::string	Server::writeLoop(int & fd, std::string str)
{
	int		rc;
	char	buffer[BUFFSIZE + 1];

	while (this->haveN(str) == false)
	{
		rc = recv(fd, buffer, sizeof(buffer), 0);
		buffer[rc] = '\0';
		str += buffer;
	}
	return str;
}

void	Server::passUser(int & fd, const char * message, User * user)
{
	std::string	rpl_error;
	if (strncmp(message, "PASS", 4) != 0)
	{
		rpl_error = ":127.0.0.1 421 PASS :Unknown command, expected PASS\r\n";
		send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
		send_log(user->fd, rpl_error.c_str(), this);
	}
	std::string	pass = message + 5;
	if (pass.empty())
	{
		rpl_error = ":127.0.0.1 461 PASS :Not enough parameters\r\n";
		send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
		send_log(user->fd, rpl_error.c_str(), this);
	}
	if (pass != this->password)
	{
		rpl_error = ":127.0.0.1 464 PASS :Password incorrect\r\n";
		send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
		send_log(user->fd, rpl_error.c_str(), this);
	}
	if (rpl_error.empty() == false)
		throw (PassException());
	user->conStep++;
}

void	Server::capUser(int & fd, const char * message, User * user)
{
	std::string	rpl_error;
	if (strncmp(message, "CAP", 3) != 0)
	{
		rpl_error = ":127.0.0.1 421 CAP :Unknown command, expected CAP\r\n";
		send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
		send_log(user->fd, rpl_error.c_str(), this);
	}
	if (rpl_error.empty() == false)
		throw (CapException());
	user->conStep++;
}

void	Server::nickUser(int & fd, const char * message, User * user)
{
	std::string	rpl_error;
	if (strncmp(message, "NICK", 4) != 0)
	{
		rpl_error = ":127.0.0.1 421 NICK :Unknown command, expected NICK\r\n";
		send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
		send_log(user->fd, rpl_error.c_str(), this);
	}
	std::string	nick = message + 5;
	if (nick.empty())
	{
		rpl_error = ":127.0.0.1 431 NICK :No nickname given\r\n";
		send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
		send_log(user->fd, rpl_error.c_str(), this);
	}
	if (this->checkNick(fd, nick) != 0)
	{
		std::ostringstream	line;
		int	rc = this->checkNick(fd, nick);
		rc == 432 ? line << rc << " :Erroneus nickname\r\n" : line << rc << " :Nickname is already in use\r\n";
		rpl_error = ":127.0.0.1 " + line.str();
		send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
		send_log(user->fd, rpl_error.c_str(), this);
	}
	if (rpl_error.empty() == false)
		throw (NickException());
	user->conStep++;
	user->nickname = nick;
}

void	Server::userUser(int & fd, const char * message, User * user)
{
	std::string	rpl_error;
	if (strncmp(message, "USER", 4) != 0)
	{
		rpl_error = ":127.0.0.1 421 USER :Unknown command, expected USER\r\n";
		send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
		send_log(user->fd, rpl_error.c_str(), this);
	}
	std::string	line = message + 5;
	if (line.empty() || line.find(':') == std::string::npos)
	{
		rpl_error = ":127.0.0.1 461 USER :Not enough parameters\r\n";
		send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
		send_log(user->fd, rpl_error.c_str(), this);
	}
	std::string	username = line.substr(0, line.find(' '));
	line.erase(0, line.find(' ') + 1);
	if ((line[0] != '0' || line[1] != ' ' || line[2] != '*') && rpl_error.empty())
	{
		rpl_error = ":127.0.0.1 461 USER :Not enough parameters\r\n";
		send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
		send_log(user->fd, rpl_error.c_str(), this);
	}
	line.erase(0, 5);
	std::string	realname = line;
	if (rpl_error.empty() == false)
		throw (UserException());
	user->username = username;
	user->realname = realname;
}

int		Server::checkNick(int & fd, std::string nickname)
{
	if (nickname.find('#') != std::string::npos || nickname.find(' ') != std::string::npos || nickname.find(':') != std::string::npos)
		return 432;
	std::vector<User *>::iterator	it = this->users.begin();
	while (it != this->users.end())
	{
		if ((*it)->fd != fd && (*it)->nickname == nickname)
			return 433;
		it++;
	}
	return 0;
}