/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 11:54:12 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/11 16:38:35 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

void	irc_join(char *message, User *user, Server *server)
{
	message = message + 6;
	message = strtok(message, "\r\n");
	for (std::vector<Channel *>::iterator it = server->channels.begin(); it != server->channels.end(); ++it)
	{
		if ((*it)->name == message)
		{
			(*it)->users.push_back(user);
			std::string rpl_join = ":" + user->nickname + " JOIN " + (*it)->name + "\r\n";
			send(user->fd, rpl_join.c_str(), rpl_join.length(), 0);
			return ;
		}
	}
	Channel *channel = new Channel(message);
	channel->users.push_back(user);
	server->channels.push_back(channel);
	std::string rpl_join = ":" + user->nickname + " JOIN " + channel->name + "\r\n";
	send(user->fd, rpl_join.c_str(), rpl_join.length(), 0);
}

void	irc_privmsg(char *message, User *user, Server *server)
{
	message = message + 8;
	message = strtok(message, "\r\n");
	int	i = 0;
	while (message[i] != ':')
		i++;
	std::string dest = message;
	dest = dest.substr(0, i - 1);
	std::cout << "DEST: |" << dest << "|" << std::endl;
	message = message + i + 1;
	std::cout << "PRIVMSG: |" << message << "|" << std::endl;
	for (std::vector<Channel *>::const_iterator it = server->channels.begin(); it != server->channels.end(); it++)
	{
		if (dest == (*it)->name)
		{
			std::string rpl_privmsg = ":" + user->nickname + " PRIVMSG " + (*it)->name + " :" + message + "\r\n";
			for (std::vector<User *>::const_iterator it2 = (*it)->users.begin(); it2 != (*it)->users.end(); it2++)
			{
				if ((*it2)->fd != user->fd)
					send((*it2)->fd, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
			}
			return ;
		}
	}
	for (std::vector<User *>::const_iterator it = server->users.begin(); it != server->users.end(); it++)
	{
		if (dest == (*it)->nickname)
		{
			std::string rpl_privmsg = ":" + user->nickname + " PRIVMSG " + (*it)->nickname + " :" + message + "\r\n";
			send((*it)->fd, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
			return ;
		}
	}
}

void	irc_part(char *message, User *user, Server *server)
{
	message = message + 6;
	message = strtok(message, "\r\n");
	for (std::vector<Channel *>::iterator it = server->channels.begin(); it != server->channels.end(); ++it)
	{
		if ((*it)->name == message)
		{
			for (std::vector<User *>::iterator it2 = (*it)->users.begin(); it2 != (*it)->users.end(); ++it2)
			{
				if ((*it2)->fd == user->fd)
				{
					(*it)->users.erase(it2);
					std::string rpl_part = ":" + user->nickname + " PART " + (*it)->name + "\r\n";
					send(user->fd, rpl_part.c_str(), rpl_part.length(), 0);
					if ((*it)->users.size() == 0)
					{
						server->channels.erase(it);
						delete (*it);
					}
					return ;
				}
			}
		}
	}
}
