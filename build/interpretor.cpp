/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpretor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:21:37 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/10 14:51:01 by rrodor           ###   ########.fr       */
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
		std::cout << "JOIN command" << std::endl;
		irc_join(message, user, server);

		std::string rpl_list = "CHANNELLIST ";
		for (std::vector<Channel *>::iterator it = server->channels.begin(); it != server->channels.end(); ++it)
		{
			rpl_list += "|" + (*it)->name + "|";
		}
		rpl_list += "\r\n";
		std::cout << "list " << rpl_list << std::endl;
	}
	else if (strncmp(message, "PRIVMSG", 7) == 0)
	{
		std::cout << "PRIVMSG command" << std::endl;
		irc_privmsg(message, user, server);
	}
	else
		send(user->fd, "Command not found", 17, 0);
}

void	interpretor(char *message, int fd, Server * server)
{
	std::cout << "Message from " << fd << " : " << message << std::endl;
	User *user = findUser(fd, server);
	if (user == NULL)
	{
		std::cout << "[ERROR] : User not found" << std::endl;
		return ;
	}
	if (message[0] >= 'A' && message[0] <= 'Z')
	{
		std::cout << "Command" << std::endl;
		commands(message, user, server);
	}
	else
		std::cout << "Message from " << user->nickname << " : " << message << std::endl;
}
