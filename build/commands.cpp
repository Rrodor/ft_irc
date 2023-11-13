/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 11:54:12 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/13 19:50:14 by rrodor           ###   ########.fr       */
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
			std::string rpl_join = ":" + user->nickname + " JOIN #" + (*it)->name + "\r\n";
			for (std::vector<User *>::iterator it2 = (*it)->users.begin(); it2 != (*it)->users.end(); ++it2)
			{
				send((*it2)->fd, rpl_join.c_str(), rpl_join.length(), 0);
			}
			for (std::vector<User *>::iterator it2 = (*it)->operators.begin(); it2 != (*it)->operators.end(); ++it2)
			{
				send((*it2)->fd, rpl_join.c_str(), rpl_join.length(), 0);
			}
			std::string ulist = "USER LIST ";
			for (std::vector<User *>::iterator it2 = (*it)->users.begin(); it2 != (*it)->users.end(); ++it2)
			{
				ulist += "|" + (*it2)->nickname + "| ";
			}
			ulist += "\n";
			std::cout << ulist << std::endl;
			std::string olist = "OPERATOR LIST ";
			for (std::vector<User *>::iterator it3 = (*it)->operators.begin(); it3 != (*it)->operators.end(); ++it3)
			{
				olist += "|" + (*it3)->nickname + "| ";
			}
			olist += "\n";
			std::cout << olist << std::endl;

			irc_names(*it, user, server);
			return ;
		}
	}
	Channel *channel = new Channel(message);
	channel->operators.push_back(user);
	server->channels.push_back(channel);
	std::string rpl_join = ":" + user->nickname + " JOIN #" + channel->name + "\r\n";
	send(user->fd, rpl_join.c_str(), rpl_join.length(), 0);

	std::string ulist = "USER LIST ";
	for (std::vector<User *>::iterator it2 = channel->users.begin(); it2 != channel->users.end(); ++it2)
	{
		ulist += "|" + (*it2)->nickname + "| ";
	}
	ulist += "\n";
	std::cout << ulist << std::endl;
	std::string olist = "OPERATOR LIST ";
	for (std::vector<User *>::iterator it3 = channel->operators.begin(); it3 != channel->operators.end(); ++it3)
	{
		olist += "|" + (*it3)->nickname + "| ";
	}
	olist += "\n";
	std::cout << olist << std::endl;

	irc_names(channel, user, server);
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
			for (std::vector<User *>::const_iterator it3 = (*it)->operators.begin(); it3 != (*it)->operators.end(); it3++)
			{
				if ((*it3)->fd != user->fd)
					send((*it3)->fd, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
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

void	irc_names(Channel *channel, User *user, Server *server)
{
	std::string rpl_names = ":127.0.0.1 353 = #" + channel->name + " :";
	for (std::vector<User *>::iterator it = channel->operators.begin(); it != channel->operators.end(); ++it)
	{
		rpl_names += "@" + (*it)->nickname + " ";
	}
	for (std::vector<User *>::iterator it = channel->users.begin(); it != channel->users.end(); ++it)
	{
		rpl_names += (*it)->nickname + " ";
	}
	std::cout << rpl_names << std::endl;
	rpl_names += "\r\n";
	send(user->fd, rpl_names.c_str(), rpl_names.length(), 0);
	std::string rpl_endnames = ":127.0.0.1 366 " + user->nickname + " #" + channel->name + " :End of /NAMES list.\r\n";
	send(user->fd, rpl_endnames.c_str(), rpl_endnames.length(), 0);
	std::cout << rpl_endnames << std::endl;
}
