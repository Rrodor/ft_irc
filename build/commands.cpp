/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 11:54:12 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/18 11:47:44 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

void	irc_join(char *message, User *user, Server *server)
{
	std::cout << COMMAND << "JOIN" << std::endl;
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
			(*it)->printChannelUsers(JOIN);
			server->printServerChannels((*it)->name);
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

	channel->printChannelUsers(JOIN);
	server->printServerChannels(channel->name);
	irc_names(channel, user, server);
}

void	irc_privmsg(char *message, User *user, Server *server)
{
	std::cout << COMMAND << "PRIVMSG" << std::endl;
	message = message + 8;
	message = strtok(message, "\r\n");
	int	i = 0;
	while (message[i] != ':')
		i++;
	std::string dest = message;
	dest = dest.substr(0, i - 1);
	if (dest[0] == '#' && !(*server->getChannelByName(dest))->isInChannel(user) && !(*server->getChannelByName(dest))->isOpInChannel(user))
		return;
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
	std::cout << COMMAND << "PART" << std::endl;
	message = message + 6;
	message = strtok(message, "\r\n");
	for (std::vector<Channel *>::iterator it = server->channels.begin(); it != server->channels.end(); ++it)
	{
		if ((*it)->name == message)
		{
			for (std::vector<User *>::iterator it2 = (*it)->users.begin(); it2 != (*it)->users.end(); ++it2)
			{
				std::cout << "user->name = " << user->nickname << std::endl;
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
				}
				else
				{
					exit(0);
					std::string rpl_part2 = ":" + user->nickname + " PART " + (*it)->name + "\r\n";
					std::cout << "rpl part" << rpl_part2 << std::endl;
					send((*it2)->fd, rpl_part2.c_str(), rpl_part2.length(), 0);
					//send_log((*it2)->fd, rpl_part.c_str(), server);
				}
			}
		}
	}
}

void	irc_names(Channel *channel, User *user, Server *server)
{
	std::cout << COMMAND << "NAMES" << std::endl;
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
	std::cout << COMMAND << "QUIT" << std::endl;
	message = message + 6;
	message = strtok(message, "\r\n");
	std::string rpl_quit = ":" + user->nickname + " QUIT :Quit: " + message + "\r\n";

	std::vector<Channel *>::iterator	it = server->channels.begin();
	std::vector<Channel *>::iterator	ite = server->channels.end();

	while (it != ite)
	{
		if ((*it)->isInChannel(user) || (*it)->isOpInChannel(user))
		{
			(*it)->channelSendLoop(rpl_quit, user->fd, server, 0);
			(*it)->deleteChannelUser(user, server);
			(*it)->printChannelUsers(QUIT);
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
	delete user;

}

void	irc_topic(char *message, User *user, Server *server)
{
	std::vector<Channel *>::iterator	it = server->channels.begin();
	std::vector<Channel *>::iterator	ite = server->channels.end();
	std::string targTopic;
	std::string targTime;
	std::time_t t = std::time(0);
	std::ostringstream oss;
	oss << t;
	std::string time_str = oss.str();
	
	if (strchr(message, ':') == 0)
	{
		while (it != ite)
		{	
			if ((*it)->name == message + 7)
			{
				if ((*it)->topic == "")
					targTopic = "No topic is set.";
				else
					targTopic = (*it)->topic;
				targTime = (*it)->lastTopicUpdate;
			}
			it++;
		}
		message = message + 6;
		std::string rpl_topic = ":127.0.0.1 332 " + user->nickname + " " + message + " :" + targTopic + "\r\n";
		std::string rpl_time = ":127.0.0.1 333 " + user->nickname + " " + message + " " + user->nickname + " " + targTime + "\r\n";
		send(user->fd, rpl_topic.c_str(), rpl_topic.length(), 0);
		send_log(user->fd, rpl_topic.c_str(), server);
		send(user->fd, rpl_time.c_str(), rpl_time.length(), 0);
		send_log(user->fd, rpl_time.c_str(), server);
		return ;
	}
	for (int i = 0; i < strlen(message); i++)
	{
		if (message[i] == ':')
		{
			message = strndup(message + i + 1, strlen(message) - i - 1);
			break ;
		}
	}
	char *targetChannel = strtok(message, " 	");
	for (std::vector<Channel *>::iterator it = server->channels.begin(); it != server->channels.end(); ++it)
	{
		if ((*it)->name == targetChannel)
		{
			if (!(*it)->isInChannel(user) && !(*it)->isOpInChannel(user))
			{
				std::string rpl_topic = ":127.0.0.1 442 " + user->nickname + " #" + targetChannel + " :You're not on that channel\r\n";
				send(user->fd, rpl_topic.c_str(), rpl_topic.length(), 0);
				send_log(user->fd, rpl_topic.c_str(), server);
				return ;
			}
			else if (!(*it)->isOpInChannel(user))
			{
				std::string rpl_not_op = ":127.0.0.1 442 " + user->nickname + " #" + targetChannel + " :You're not channel operator\r\n";
				send(user->fd, rpl_not_op.c_str(), rpl_not_op.length(), 0);
				send_log(user->fd, rpl_not_op.c_str(), server);
				return ;
			}
		}
	}
	char *topic = strtok(NULL, "\r\n");
	if (topic == NULL)
		topic = strdup("");
	for (std::vector<Channel *>::iterator it = server->channels.begin(); it != server->channels.end(); ++it)
	{
		if ((*it)->name == targetChannel)
		{
			(*it)->topic = topic;
			(*it)->lastTopicUpdate = time_str;
			std::string rpl_topic = ":" + user->nickname + " TOPIC #" + (*it)->name + " :" + topic + "\r\n";
		}
	}
	std::string rpl_topic = ":" + user->nickname + " TOPIC #" + targetChannel + " :" + topic + "\r\n";
	while (it != ite)
	{
		if ((*it)->name == targetChannel)
		{
			std::vector<User *>::iterator	it2 = (*it)->users.begin();
			while (it2 != (*it)->users.end())
			{
				send((*it2)->fd, rpl_topic.c_str(), rpl_topic.length(), 0);
				send_log((*it2)->fd, rpl_topic.c_str(), server);
				it2++;
			}
			it2 = (*it)->operators.begin();
			while (it2 != (*it)->operators.end())
			{
				send((*it2)->fd, rpl_topic.c_str(), rpl_topic.length(), 0);
				send_log((*it2)->fd, rpl_topic.c_str(), server);
				it2++;
			}
		}
		it++;
	}
}

void	irc_nick(char *message, User *user, Server *server)
{
	std::cout << COMMAND << "NICK" << std::endl;
	message = message + 5;
	message = strtok(message, "\r\n");
	std::string	oldNick = user->nickname;
	user->nickname = message;
	std::string rpl_nick = ":" + oldNick + " NICK " + message + "\r\n";

	std::vector<Channel *>::iterator	it = server->channels.begin();
	std::vector<Channel *>::iterator	ite = server->channels.end();

	while (it != ite)
	{
		if ((*it)->isInChannel(user) || (*it)->isOpInChannel(user))
			(*it)->channelSendLoop(rpl_nick, user->fd, server, 1);
		it++;
	}
}

void	irc_mode(char *message, User *user, Server *server)
{
	std::cout << COMMAND << "MODE" << std::endl;
	message = message + 6;
	std::string rpl_mode = ":127.0.0.1 " + user->nickname + " #" + message + "\r\n";
	// send(user->fd, rpl_mode.c_str(), rpl_mode.length(), 0);
	// send_log(user->fd, rpl_mode.c_str(), server);
}

void	irc_kick(char * message, User * user, Server * server)
{
	std::string	reason;
	bool		hasReason = false;	

	std::cout << COMMAND << "KICK" << std::endl;

	message = message + 5;
	message = strtok(message, "\r\n");
	std::string	rpl_kick = message;
	if (rpl_kick.find(":") != std::string::npos)
		hasReason = true;
	std::string channelName = rpl_kick.substr(0, rpl_kick.find(' '));
	rpl_kick.erase(0, rpl_kick.find(' ') + 1);
	std::string kickedUser = rpl_kick.substr(0, rpl_kick.find(' '));
	reason = "Kicked by operator";
	if (hasReason == true)
	{
		rpl_kick.erase(0, rpl_kick.find(' ') + 2);
		reason = rpl_kick;
	}
	channelName.erase(channelName.begin());
	std::vector<Channel *>::iterator	it = server->getChannelByName(channelName);
	if (it == server->channels.end())
		return;
	if (!(*it)->isOpInChannel(user))
		return;
	rpl_kick = ":" + user->nickname + " KICK #" + channelName + " " + kickedUser + " :" + reason + "\r\n";
	(*it)->channelSendLoop(rpl_kick, user->fd, server, 1);
	(*it)->deleteChannelUser((*it)->getUserByNick(kickedUser), server);
}