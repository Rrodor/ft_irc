/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpretor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:21:37 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/21 12:31:51 by babreton         ###   ########.fr       */
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
		irc_join(message, user, server);
	else if (strncmp(message, "PRIVMSG", 7) == 0)
		irc_privmsg(message, user, server);
	else if (strncmp(message, "PING", 4) == 0)
	{
		std::cout << COMMAND << "PING" << std::endl;
		std::string rpl_pong = "PONG " + std::string(message + 5) + "\r\n";
		send(user->fd, rpl_pong.c_str(), rpl_pong.length(), 0);
		send_log(user->fd, rpl_pong.c_str(), server);
	}
	else if (strncmp(message , "PART", 4) == 0)
		irc_part(message, user, server);
	else if (strncmp(message , "MODE", 4) == 0)
		irc_mode(message, user, server);
	else if (strncmp(message , "QUIT", 4) == 0)
		irc_quit(message, user, server);
	else if (strncmp(message , "NICK", 4) == 0)
		irc_nick(message, user, server);
	else if (strncmp(message, "TOPIC", 5) == 0)
		irc_topic(message, user, server);
	else if (strncmp(message, "KICK", 4) == 0)
		irc_kick(message, user, server);
	else if (strncmp(message, "INVITE", 6) == 0)
		irc_invite(message, user, server);
	else
	{
		std::string	rpl_error = ":127.0.0.1 421 " + user->nickname + " :Unknown command\r\n";
		send(user->fd, rpl_error.c_str(), rpl_error.length(), 0);
		send_log(user->fd, rpl_error.c_str(), server);
	}
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
