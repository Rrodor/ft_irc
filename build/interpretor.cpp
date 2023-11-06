/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpretor.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:21:37 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/06 18:46:14 by rrodor           ###   ########.fr       */
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
	if (strncmp(message, "/JOIN", 5) == 0)
	{
		Channel *channel = new Channel(message + 6);
		channel->users.push_back(user);
		server->channels.push_back(channel);
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
	if (message[0] == '/')
		commands(message, user, server);
	else
		std::cout << "Message from " << user->nickname << " : " << message << std::endl;
}
