/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 11:54:12 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/15 14:47:44 by babreton         ###   ########.fr       */
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
				send_log((*it2)->fd, rpl_join.c_str(), server);
			}
			for (std::vector<User *>::iterator it2 = (*it)->operators.begin(); it2 != (*it)->operators.end(); ++it2)
			{
				send((*it2)->fd, rpl_join.c_str(), rpl_join.length(), 0);
				send_log((*it2)->fd, rpl_join.c_str(), server);
			}
			std::string ulist = JOIN;
			ulist += "[#" + (*it)->name + "]" + "[USERS_LIST] > ";
			for (std::vector<User *>::iterator it2 = (*it)->users.begin(); it2 != (*it)->users.end(); ++it2)
			{
				ulist += "|" + (*it2)->nickname + "| ";
			}
			std::cout << ulist << std::endl;
			std::string olist = JOIN;
			olist += "[#" + (*it)->name + "]" + "[OPERATORS_LIST] > ";
			for (std::vector<User *>::iterator it3 = (*it)->operators.begin(); it3 != (*it)->operators.end(); ++it3)
			{
				olist += "|" + (*it3)->nickname + "| ";
			}
			std::cout << olist << std::endl;
			std::string	clist = JOIN;
			clist += "[#" + (*it)->name + "]" + "[CHANNELS_LIST] > ";
			for (std::vector<Channel *>::iterator it = server->channels.begin(); it != server->channels.end(); ++it)
			{
				clist += "|" + (*it)->name + "|";
			}
			std::cout << clist << std::endl;
			irc_names(*it, user, server);
			return ;
		}
	}
	Channel *channel = new Channel(message);
	channel->operators.push_back(user);
	server->channels.push_back(channel);
	std::string rpl_join = ":" + user->nickname + " JOIN #" + channel->name + "\r\n";
	send(user->fd, rpl_join.c_str(), rpl_join.length(), 0);
	send_log(user->fd, rpl_join.c_str(), server);

	std::string ulist = JOIN;
	ulist += "[#" + channel->name + "]" + "[USERS_LIST] > ";
	for (std::vector<User *>::iterator it2 = channel->users.begin(); it2 != channel->users.end(); ++it2)
	{
		ulist += "|" + (*it2)->nickname + "| ";
	}
	std::cout << ulist << std::endl;
	std::string olist = JOIN;
	olist += "[#" + channel->name + "]" + "[OPERATORS_LIST] > ";
	for (std::vector<User *>::iterator it3 = channel->operators.begin(); it3 != channel->operators.end(); ++it3)
	{
		olist += "|" + (*it3)->nickname + "| ";
	}
	std::cout << olist << std::endl;
	std::string	clist = JOIN;
	clist += "[#" + channel->name + "]" + "[CHANNELS_LIST] > ";
	for (std::vector<Channel *>::iterator it = server->channels.begin(); it != server->channels.end(); ++it)
	{
		clist += "|" + (*it)->name + "|";
	}
	std::cout << clist << std::endl;
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
	std::cout << PRIVMSG << "sent to " << dest << std::endl;
	message = message + i + 1;
	std::cout << PRIVMSG << "\"" << message << "\"" << RESET << std::endl;
	if (dest[0] == '#')
	{
		dest = dest.substr(1, dest.length());
		for (std::vector<Channel *>::const_iterator it = server->channels.begin(); it != server->channels.end(); it++)
		{
			if (dest == (*it)->name)
			{
				std::string rpl_privmsg = ":" + user->nickname + " PRIVMSG #" + (*it)->name + " :" + message + "\r\n";
				for (std::vector<User *>::const_iterator it2 = (*it)->users.begin(); it2 != (*it)->users.end(); it2++)
				{
					if ((*it2)->fd != user->fd)
					{
						send((*it2)->fd, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
						send_log((*it2)->fd, rpl_privmsg.c_str(), server);
					}
				}
				for (std::vector<User *>::const_iterator it3 = (*it)->operators.begin(); it3 != (*it)->operators.end(); it3++)
				{
					if ((*it3)->fd != user->fd)
					{
						send((*it3)->fd, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
						send_log((*it3)->fd, rpl_privmsg.c_str(), server);
					}
				}
				return ;
			}
		}
	}
	else
	{
		for (std::vector<User *>::const_iterator it = server->users.begin(); it != server->users.end(); it++)
		{
			if (dest == (*it)->nickname)
			{
				std::string rpl_privmsg = ":" + user->nickname + " PRIVMSG " + (*it)->nickname + " :" + message + "\r\n";
				send((*it)->fd, rpl_privmsg.c_str(), rpl_privmsg.length(), 0);
				send_log((*it)->fd, rpl_privmsg.c_str(), server);
				return ;
			}
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
					send_log(user->fd, rpl_part.c_str(), server);
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
	rpl_names += "\r\n";
	send(user->fd, rpl_names.c_str(), rpl_names.length(), 0);
	send_log(user->fd, rpl_names.c_str(), server);
	std::string rpl_endnames = ":127.0.0.1 366 " + user->nickname + " #" + channel->name + " :End of /NAMES list.\r\n";
	send(user->fd, rpl_endnames.c_str(), rpl_endnames.length(), 0);
	send_log(user->fd, rpl_endnames.c_str(), server);
}

void	irc_quit(char *message, User *user, Server *server)
{
	message = message + 6;
	message = strtok(message, "\r\n");
	std::string rpl_quit = ":" + user->nickname + " QUIT :Quit: " + message + "\r\n";

	std::vector<Channel *>::iterator	it = server->channels.begin();
	std::vector<Channel *>::iterator	ite = server->channels.end();

	std::vector<User *>::iterator		it2;
	while (it != ite)
	{
		if ((*it)->isInChannel(user) || (*it)->isOpInChannel(user))
		{
			it2 = (*it)->users.begin();
			while (it2 != (*it)->users.end())
			{
				if (user != *it2)
				{
					send((*it2)->fd, rpl_quit.c_str(), rpl_quit.length(), 0);
					send_log((*it2)->fd, rpl_quit.c_str(), server);
				}
				it2++;
			}
			it2 = (*it)->operators.begin();
			while (it2 != (*it)->operators.end())
			{
				if (user != *it2)
				{
					send((*it2)->fd, rpl_quit.c_str(), rpl_quit.length(), 0);
					send_log((*it2)->fd, rpl_quit.c_str(), server);
				}
				it2++;
			}
		}
		it++;
	}

	std::vector<struct pollfd>::iterator	it4 = server->fds.begin();
	std::vector<struct pollfd>::iterator	it4e = server->fds.end();
	while (it4 != it4e)
	{
		if ((*it4).fd == user->fd)
		{
			close((*it4).fd);
			(*it4).fd = -1;
		}
		it4++;
	}
}


void	irc_nick(char *message, User *user, Server *server)
{
	message = message + 5;
	message = strtok(message, "\r\n");
	std::string	oldNick = user->nickname;
	user->nickname = message;
	std::string rpl_nick = ":" + oldNick + " NICK " + message + "\r\n";

	std::vector<Channel *>::iterator	it = server->channels.begin();
	std::vector<Channel *>::iterator	ite = server->channels.end();

	std::vector<User *>::iterator		it2;

	while (it != ite)
	{
		if ((*it)->isInChannel(user) || (*it)->isOpInChannel(user))
		{
			it2 = (*it)->users.begin();
			while (it2 != (*it)->users.end())
			{
				send((*it2)->fd, rpl_nick.c_str(), rpl_nick.length(), 0);
				send_log((*it2)->fd, rpl_nick.c_str(), server);
				it2++;
			}
			it2 = (*it)->operators.begin();
			while (it2 != (*it)->operators.end())
			{
				send((*it2)->fd, rpl_nick.c_str(), rpl_nick.length(), 0);
				send_log((*it2)->fd, rpl_nick.c_str(), server);
				it2++;
			}
		}
		it++;
	}
}

void	irc_mode(char *message, User *user, Server *server)
{
	message = message + 6;
	std::string rpl_mode = ":127.0.0.1 #" + user->nickname + " #" + message + "\r\n";
	send(user->fd, rpl_mode.c_str(), rpl_mode.length(), 0);
	send_log(user->fd, rpl_mode.c_str(), server);
}
