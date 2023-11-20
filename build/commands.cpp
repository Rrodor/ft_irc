/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/10 11:54:12 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/20 15:45:35 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

void	irc_join_cases(char *message, User *user, Server *server)
{
	int flag = 0;
	if (message[0] == '0')
	{
		std::vector<Channel *>::iterator	it = server->channels.begin();
		std::vector<Channel *>::iterator	ite = server->channels.end();
		while (it != ite)
		{
			if ((*it)->isInChannel(user) || (*it)->isOpInChannel(user))
			{
				flag = 1;
				std::string	channelName = (*it)->name;
				std::string rpl_part = ":" + user->nickname + " PART #" + (*it)->name + " :Left all channels\r\n";
				(*it)->channelSendLoop(rpl_part, user->fd, server, 1);
				(*it)->nbUsers--;
				(*it)->deleteChannelUser(user, server, 0);
			}
			it++;
		}
		if (flag == 0)
		{
			std::string rpl_not_valid_name = ":127.0.0.1 476 " + user->nickname + " " + message + " :Invalid channel name\r\n";
			send(user->fd, rpl_not_valid_name.c_str(), rpl_not_valid_name.length(), 0);
			send_log(user->fd, rpl_not_valid_name.c_str(), server);
		}
		server->checkChannel();
		return ;
	}
	else
	{
		std::string rpl_not_valid_name = ":127.0.0.1 476 " + user->nickname + " " + message + " :Invalid channel name\r\n";
		send(user->fd, rpl_not_valid_name.c_str(), rpl_not_valid_name.length(), 0);
		send_log(user->fd, rpl_not_valid_name.c_str(), server);
		return ;
	}
}

void	join_rmInvit(Channel *channel, User *user)
{
	for (std::vector<User *>::iterator it = channel->invitedUsers.begin(); it != channel->invitedUsers.end(); ++it)
	{
		if ((*it)->fd == user->fd)
		{
			channel->invitedUsers.erase(it);
			return ;
		}
	}
}

char	*parse_join(char *message, std::string &passwd)
{
	int i = 0;
	int j = 0;
	while (message[i] != ' ' && message[i] != '\0')
		i++;
	passwd = message;
	if (i + 1 < (int)passwd.length())
		passwd = passwd.substr(i + 1, passwd.length());
	else
		passwd = "";
	message[i] = '\0';
	std::cout << JOIN << " > Try to join channel " << message << RESET << std::endl;
	std::cout << JOIN << " > Channel password : " << passwd << RESET << std::endl;
	return message;
}

void	irc_join(char *message, User *user, Server *server)
{
	std::string passwd;
	std::cout << COMMAND << "JOIN" << std::endl;
	message = message + 5;
	message = parse_join(message, passwd);
	if (message[0] != '#')
		return irc_join_cases(message, user, server);
	message = message + 1;
	message = strtok(message, "\r\n");
	for (std::vector<Channel *>::iterator it = server->channels.begin(); it != server->channels.end(); ++it)
	{
		if ((*it)->name == message)
		{
			if ((*it)->isModeI())
			{
				if (!(*it)->isInvited(user))
				{
					std::string rpl_not_invited = ":127.0.0.1 473 " + user->nickname + " #" + (*it)->name + " :Cannot join channel (+i)\r\n";
					send(user->fd, rpl_not_invited.c_str(), rpl_not_invited.length(), 0);
					send_log(user->fd, rpl_not_invited.c_str(), server);
					return;
				}
			}
			if ((*it)->isModeL())
			{
				if ((*it)->nbUsers >= (*it)->maxUsers)
				{
					std::string rpl_channel_full = ":127.0.0.7 471 " + user->nickname + " #" + (*it)->name + " :Cannot join channel (+l)\r\n";
					send(user->fd, rpl_channel_full.c_str(), rpl_channel_full.length(), 0);
					send_log(user->fd, rpl_channel_full.c_str(), server);
					return;
				}
			}
			if ((*it)->isModeK() && !(*it)->isInvited(user))
			{
				if ((*it)->password != passwd)
				{
					std::string rpl_wrong_passwd = ":127.0.0.1 475 " + user->nickname + " #" + (*it)->name + " :Cannot join channel (+k)\r\n";
					send(user->fd, rpl_wrong_passwd.c_str(), rpl_wrong_passwd.length(), 0);
					send_log(user->fd, rpl_wrong_passwd.c_str(), server);
					return;
				}
			}
			(*it)->users.push_back(user);
			join_rmInvit((*it), user);
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
			std::string topic_mess = "TOPIC #" + (*it)->name + "\r\n";
			char *topic = strdup(topic_mess.c_str());
			irc_topic(topic, user, server);
			irc_names(*it, user, server);
			(*it)->nbUsers++;
			return ;
		}
	}
	Channel *channel = new Channel(message);
	channel->lastTopicUpdateWho = user->nickname;
	channel->nbUsers = 1;
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
	int			rc;
	std::cout << COMMAND << "PART" << std::endl;

	message = message + 6;
	message = strtok(message, "\r\n");
	std::string rpl_part = message;
	std::string	channelName = rpl_part.substr(0, rpl_part.find(' '));
	rpl_part.erase(0, rpl_part.find(' ') + 2);
	std::string	reason = rpl_part;
	rpl_part = ":" + user->nickname + " PART #" + channelName + " :" + rpl_part + "\r\n";

	std::vector<Channel *>::iterator	it = server->channels.begin();
	std::vector<Channel *>::iterator	ite = server->channels.end();

	while (it != ite)
	{
		if ((*it)->name == channelName)
		{
			(*it)->channelSendLoop(rpl_part, user->fd, server, 1);
			(*it)->nbUsers--;
			rc = (*it)->deleteChannelUser(user, server);
			if (rc == 1)
			{
				delete (*it);
				std::cout << DELETE << "Succesfully deleted channel " << "#" << channelName << RESET << std::endl;
			}
		}
		it++;
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
	channel->channelSendLoop(rpl_names, user->fd, server, 1);
	std::string rpl_endnames = ":127.0.0.1 366 " + user->nickname + " #" + channel->name + " :End of /NAMES list.\r\n";
	channel->channelSendLoop(rpl_endnames, user->fd, server, 1);
}

void	irc_quit(char *message, User *user, Server *server)
{
	std::cout << COMMAND << "QUIT" << std::endl;
	message = message + 6;
	message = strtok(message, "\r\n");
	std::string rpl_quit = ":" + user->nickname + " QUIT :Quit: " + message + "\r\n";

	std::vector<Channel *>::iterator	it = server->channels.begin();
	std::vector<Channel *>::iterator	ite = server->channels.end();
	int									rc;

	while (it != ite)
	{
		if ((*it)->isInChannel(user) || (*it)->isOpInChannel(user))
		{
			(*it)->channelSendLoop(rpl_quit, user->fd, server, 0);
			(*it)->nbUsers--;
			rc = (*it)->deleteChannelUser(user, server);
			if (rc == 1)
			{
				std::string name = (*it)->name;
				delete (*it);
				std::cout << DELETE << "Succesfully deleted channel " << "#" << name << RESET << std::endl;
			}
			else
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
	std::string channel;
	std::string topic;
	int i = 0;
	std::vector<Channel *>::iterator	it;
	std::vector<Channel *>::iterator	ite = server->channels.end();
	std::time_t t = std::time(0);
	std::ostringstream oss;
	oss << t;
	std::string time_str = oss.str();

	std::cout << COMMAND << "TOPIC" << std::endl;
	message = message + 6;
	message = strtok(message, "\r\n");
	while (message[i] != ' ' && message[i] != '\0')
		i++;
	channel = message;
	channel = channel.substr(1, i - 1);
	it = server->getChannelByName(channel);
	if (it == ite)
	{
		std::string rpl_not_valid_name = ":127.0.0.1 403 " + user->nickname + " #" + channel + " :No such channel\r\n";
		send(user->fd, rpl_not_valid_name.c_str(), rpl_not_valid_name.length(), 0);
		send_log(user->fd, rpl_not_valid_name.c_str(), server);
		return ;
	}
	if (!(*it)->isInChannel(user) && !(*it)->isOpInChannel(user))
	{
		std::string rpl_not_valid_name = ":127.0.0.1 442 " + user->nickname + " #" + channel + " :You're not on that channel\r\n";
		send(user->fd, rpl_not_valid_name.c_str(), rpl_not_valid_name.length(), 0);
		send_log(user->fd, rpl_not_valid_name.c_str(), server);
		return ;
	}
	if (message[i] == '\0')
	{
		if ((*it)->topic == "")
		{
			std::string rpl_no_topic = ":127.0.0.1 331 " + user->nickname + " #" + channel + " :No topic is set\r\n";
			send(user->fd, rpl_no_topic.c_str(), rpl_no_topic.length(), 0);
			send_log(user->fd, rpl_no_topic.c_str(), server);
			return ;
		}
		else
		{
			std::string rpl_topic = ":127.0.0.1 332 " + user->nickname + " #" + channel + " :" + (*it)->topic + "\r\n";
			send(user->fd, rpl_topic.c_str(), rpl_topic.length(), 0);
			send_log(user->fd, rpl_topic.c_str(), server);
			std::string rpl_time = ":127.0.0.1 333 " + user->nickname + " #" + channel + " " + (*it)->lastTopicUpdateWho + " " + (*it)->lastTopicUpdateWhen + "\r\n";
			send(user->fd, rpl_time.c_str(), rpl_time.length(), 0);
			send_log(user->fd, rpl_time.c_str(), server);
			return ;
		}
	}
	else
	{
		if ((*it)->isModeT())
		{
			if (!(*it)->isOpInChannel(user))
			{
				std::string rpl_not_op = ":127.0.0.1 482 " + user->nickname + " #" + channel + " :You're not channel operator\r\n";
				send(user->fd, rpl_not_op.c_str(), rpl_not_op.length(), 0);
				send_log(user->fd, rpl_not_op.c_str(), server);
				return ;
			}
		}
		message = message + i + 2;
		topic = message;
		(*it)->topic = topic;
		(*it)->lastTopicUpdateWhen = time_str;
		(*it)->lastTopicUpdateWho = user->nickname;
		std::string rpl_topic = ":" + user->nickname + " TOPIC #" + channel + " :" + topic + "\r\n";
		(*it)->channelSendLoop(rpl_topic, user->fd, server, 1);
		std::string rpl_time = ":127.0.0.1 333 " + user->nickname + " #" + channel + " " + (*it)->lastTopicUpdateWho + " " + (*it)->lastTopicUpdateWhen + "\r\n";
		(*it)->channelSendLoop(rpl_time, user->fd, server, 1);
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
	bool	hasLastParam;
	std::cout << COMMAND << "MODE" << std::endl;
	message = message + 6;
	std::string	rpl_mode = message;
	std::string	channelName = rpl_mode.substr(0, rpl_mode.find(' '));
	rpl_mode.erase(0, rpl_mode.find(' ') + 1);
	std::string	mode = rpl_mode.substr(0, rpl_mode.find(' '));
	if (channelName == mode)
		return;
	rpl_mode.erase(0, rpl_mode.find(' ') + 1);
	std::string param = rpl_mode;
	param != mode ? hasLastParam = true : hasLastParam = false;

	std::string::iterator	it = mode.begin();
	std::string::iterator	ite = mode.end();
	int						sign = -1;
	while (it != ite)
	{
		if ((*it) == '+')
			sign = 1;
		else if ((*it) == '-')
			sign = 0;
		if ((*it) == 'o' && sign == 0)
		{
			std::cout << YELLOW << "Operator " << user->nickname << " try to remove operator privilege to " << param << RESET << std::endl;
			(*server->getChannelByName(channelName))->deOpUser(user, (*server->getChannelByName(channelName))->getUserByNick(param), server);
				return;
		}
		else if ((*it) == 'o' && sign == 1)
		{
			std::cout << YELLOW << "Operator " << user->nickname << " try to set user " << param << " has new channel operator" << RESET << std::endl;
			(*server->getChannelByName(channelName))->opUser(user, (*server->getChannelByName(channelName))->getUserByNick(param), server);
				return;
		}
		else if ((*it) == 'k' && sign == 0)
		{
			(*server->getChannelByName(channelName))->password.clear();
			std::cout << YELLOW << "Operator " << user->nickname << " unset password on channel #" << channelName << RESET << std::endl;
		}
		else if ((*it) == 'k' && sign == 1)
		{
			if (hasLastParam == true)
			{
				(*server->getChannelByName(channelName))->password = param;
				std::cout << YELLOW << "Operator " << user->nickname << " set password " << param << " has new channel password." << RESET << std::endl;
			}
		}
		if ((*it) >= 'a' && (*it) <= 'z' && sign == 1)
		{
			if ((*server->getChannelByName(channelName))->mode.find((*it)) == std::string::npos)
				(*server->getChannelByName(channelName))->mode.push_back((*it));
		}
		else if ((*it) >= 'a' && (*it) <= 'z' && sign == 0)
		{
			if ((*server->getChannelByName(channelName))->mode.find((*it)) != std::string::npos)
				(*server->getChannelByName(channelName))->mode.erase((*server->getChannelByName(channelName))->mode.find((*it)));
		}
		it++;
	}
	rpl_mode = ":127.0.0.1 " + user->nickname + " #" + channelName + " " + mode;
	hasLastParam == true ? rpl_mode += " " + param + "\r\n" : rpl_mode += "\r\n";
	send(user->fd, rpl_mode.c_str(), rpl_mode.length(), 0);
	send_log(user->fd, rpl_mode.c_str(), server);
}

void	irc_kick(char * message, User * user, Server * server)
{
	std::string	reason;
	bool		hasReason = false;
	int			rc;

	std::cout << COMMAND << "KICK" << std::endl;

	message = message + 6;
	message = strtok(message, "\r\n");
	std::string	rpl_kick = message;
	if (rpl_kick.find(':') != std::string::npos)
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
	std::vector<Channel *>::iterator	it = server->getChannelByName(channelName);
	if (it == server->channels.end())
		return;
	if (!(*it)->isOpInChannel(user) || !(*it)->isBestOp(user, (*it)->getUserByNick(kickedUser)))
	{
		std::cout << ERROR << user->nickname << " is not operator or have less privileges than " << kickedUser << ", can't kick it." << RESET << std::endl;
		return;
	}
	rpl_kick = ":" + user->nickname + " KICK #" + channelName + " " + kickedUser + " :" + reason + "\r\n";
	(*it)->channelSendLoop(rpl_kick, user->fd, server, 1);
	rc = (*it)->deleteChannelUser((*it)->getUserByNick(kickedUser), server);
	if (rc == 1)
	{
		std::string name = (*it)->name;
		delete (*it);
		std::cout << DELETE << "Succesfully deleted channel " << "#" << name << RESET << std::endl;
	}
}

void	irc_invite(char *message, User *user, Server *server)
{
	int i = 0;
	std::string nick;
	std::string channel;
	message = message + 7;
	message = strtok(message, "\r\n");
	std::string strmessage = message;
	while (message[i] != ' ')
		i++;
	nick = strmessage.substr(0, i);
	channel = strmessage.substr(i + 2, strmessage.length());
	std::vector<Channel *>::iterator	it = server->getChannelByName(channel);
	if (it == server->channels.end())
	{
		std::string rpl_not_in_channel = ":127.0.0.1 403 " + user->nickname + " #" + channel + " :No such channel\r\n";
		send(user->fd, rpl_not_in_channel.c_str(), rpl_not_in_channel.length(), 0);
		send_log(user->fd, rpl_not_in_channel.c_str(), server);
		std::cout << ERROR << "No channel found." << RESET << std::endl;
	}
	else
	{
		std::vector<User *>::iterator	it2 = server->getUserByName(nick);
		if (it2 == server->users.end())
		{
			std::string	rpl_no_such_nick = ":127.0.0.1 401 " + user->nickname + " " + nick + " :No such nickname\r\n";
			send(user->fd, rpl_no_such_nick.c_str(), rpl_no_such_nick.length(), 0);
			send_log(user->fd, rpl_no_such_nick.c_str(), server);
			std::cout << ERROR << "No user found." << RESET << std::endl;
			return ;
		}
		if ((*it)->isInChannel(*it2)  || (*it)->isOpInChannel(*it2))
		{
			std::string rpl_already_in_channel = ":127.0.0.1 443 " + user->nickname + " #" + channel + " " + nick + " :is already on channel\r\n";
			send(user->fd, rpl_already_in_channel.c_str(), rpl_already_in_channel.length(), 0);
			send_log(user->fd, rpl_already_in_channel.c_str(), server);
			std::cout << ERROR << "User already in channel." << RESET << std::endl;
			return ;
		}
		std::string rpl_invite = ":" + user->nickname + " INVITE " + nick + " #" + channel + "\r\n";
		send((*it2)->fd, rpl_invite.c_str(), rpl_invite.length(), 0);
		send_log((*it2)->fd, rpl_invite.c_str(), server);
		(*it)->invitedUsers.push_back(*it2);
		std::cout << INVITE << " > " << user->nickname << " invite " << nick << " in #" << channel << "." << RESET << std::endl;
	}
}
