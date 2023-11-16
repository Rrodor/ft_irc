/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpretor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:21:37 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/16 12:28:25 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

User	*findUser(int fd, Server * server)
{
	for (std::vector<User *>::iterator it = server->users.begin(); it != server->users.end(); ++it)
	{
		if ((*it)->fd == fd)
			return (*it);
	}
	return NULL;
}

void	commands(char *message, User *user, Server *server)
{
	if (strncmp(message, "JOIN", 4) == 0)
	{
		std::cout << COMMAND << "JOIN" << std::endl;
		irc_join(message, user, server);
	}
	else if (strncmp(message, "PRIVMSG", 7) == 0)
	{
		std::cout << COMMAND << "PRIVMSG" << std::endl;
		irc_privmsg(message, user, server);
	}
	else if (strncmp(message, "PING", 4) == 0)
	{
		std::cout << COMMAND << "PING" << std::endl;
		std::string rpl_pong = "PONG " + std::string(message + 5) + "\r\n";
		send(user->fd, rpl_pong.c_str(), rpl_pong.length(), 0);
		send_log(user->fd, rpl_pong.c_str(), server);
	}
	else if (strncmp(message , "PART", 4) == 0)
	{
		std::cout << COMMAND << "PART" << std::endl;
		irc_part(message, user, server);
	}
	else if (strncmp(message , "MODE", 4) == 0)
	{
		std::cout << COMMAND << "MODE" << std::endl;
		irc_mode(message, user, server);
	}
	else if (strncmp(message , "QUIT", 4) == 0)
	{
		std::cout << COMMAND << "QUIT" << std::endl;
		irc_quit(message, user, server);
	}
	else if (strncmp(message , "NICK", 4) == 0)
	{
		std::cout << COMMAND << "NICK" << std::endl;
		irc_nick(message, user, server);
	}
	else if (strncmp(message, "TOPIC", 5) == 0)
	{
		std::cout << COMMAND << "TOPIC" << std::endl;
		irc_topic(message, user, server);
	}
	else
		send(user->fd, "Command not found", 17, 0);
}

void	interpretor(char *message, int fd, Server * server)
{
	User *user = findUser(fd, server);
	if (user == NULL)
	{
		std::cout << "[ERROR] : User not found" << std::endl;
		return ;
	}
	if (message[0] >= 'A' && message[0] <= 'Z')
		commands(message, user, server);
}
