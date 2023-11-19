/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:36:08 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/19 13:17:39 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

Channel::Channel(std::string name)
{
	this->name = name;
	this->topic = "";
	std::time_t t = std::time(0);
	std::ostringstream oss;
	oss << t;
	this->lastTopicUpdate = oss.str();
}

Channel::~Channel()
{
}

bool	Channel::isInChannel(User *user)
{
	if (!user)
		return false;

	std::vector<User *>::iterator		it = this->users.begin();
	std::vector<User *>::iterator		ite = this->users.end();

	while (it != ite)
	{
		if (user == *it)
			return true;
		it++;
	}
	return false;
}

bool	Channel::isOpInChannel(User *user)
{
	if (!user)
		return false;

	std::vector<User *>::iterator		it = this->operators.begin();
	std::vector<User *>::iterator		ite = this->operators.end();

	while (it != ite)
	{
		if (user == *it)
			return true;
		it++;
	}
	return false;
}

bool	Channel::isInvited(User *user)
{
	if (!user)
		return false;

	std::vector<User *>::iterator		it = this->invitedUsers.begin();
	std::vector<User *>::iterator		ite = this->invitedUsers.end();

	while (it != ite)
	{
		if (user == *it)
			return true;
		it++;
	}
	return false;
}


void	Channel::printChannelUsers(std::string logType) const
{
	std::string ulist = logType;
	ulist += "[#" + this->name + "]" + "[USERS_LIST] > ";
	for (std::vector<User *>::const_iterator it2 = this->users.begin(); it2 != this->users.end(); ++it2)
	{
		ulist += "|" + (*it2)->nickname + "| ";
	}
	std::cout << ulist << std::endl;
	std::string olist = logType;
	olist += "[#" + this->name + "]" + "[OPERATORS_LIST] > ";
	for (std::vector<User *>::const_iterator it3 = this->operators.begin(); it3 != this->operators.end(); ++it3)
	{
		olist += "|" + (*it3)->nickname + "| ";
	}
	std::cout << olist << std::endl;
}

void	Channel::channelSendLoop(std::string message, int & sFd, Server * server, int sendToHim)
{
	std::vector<User *>::iterator	it = this->users.begin();
	std::vector<User *>::iterator	ite = this->users.end();

	while (it != ite)
	{
		if (sFd != (*it)->fd || (sFd == (*it)->fd && sendToHim == 1))
		{
			send((*it)->fd, message.c_str(), message.length(), 0);
			send_log((*it)->fd, message.c_str(), server);
		}
		it++;
	}
	it = this->operators.begin();
	ite = this->operators.end();
	while (it != ite)
	{
		if (sFd != (*it)->fd || (sFd == (*it)->fd && sendToHim == 1))
		{
			send((*it)->fd, message.c_str(), message.length(), 0);
			send_log((*it)->fd, message.c_str(), server);
		}
		it++;
	}
}

int		Channel::deleteChannelUser(std::vector<User *>::iterator iterator, Server * server)
{
	std::vector<User *>::iterator	it = this->users.begin();
	std::vector<User *>::iterator	ite = this->users.end();

	if (this->isOpInChannel((*iterator)))
	{
		it = this->operators.begin();
		ite = this->operators.end();
	}

	while (it != ite)
	{
		if (iterator == it)
		{
			if (this->isInChannel((*iterator)))
			{
				this->users.erase(it);
				std::cout << DELETE << "Erasing user " << (*iterator)->nickname << " from #" << this->name << RESET << std::endl;
			}
			else if (this->isOpInChannel((*iterator)))
			{
				this->operators.erase(it);
				std::cout << DELETE << "Erasing operator " << (*iterator)->nickname << " from #" << this->name << RESET << std::endl;
			}
		}
		it++;
	}
	if (this->operators.empty() && !this->users.empty())
		this->allocNewOp(server);
	else if (this->operators.empty() && this->users.empty())
	{
		std::cout << DELETE << "Channel " << this->name << " is empty, erasing it from server..." << RESET << std::endl;
		server->channels.erase(server->getChannelByName(this->name));
		return 1;
	}
	return 0;
}

int	Channel::deleteChannelUser(User * user, Server * server)
{
	std::vector<User *>::iterator	it = this->users.begin();
	std::vector<User *>::iterator	ite = this->users.end();

	if (this->isOpInChannel(user))
	{
		it = this->operators.begin();
		ite = this->operators.end();
	}

	while (it != ite)
	{
		if (user == (*it))
		{
			if (this->isInChannel(user))
			{
				this->users.erase(it);
				std::cout << DELETE << "Erasing user " << user->nickname << " from #" << this->name << RESET << std::endl;
			}
			else if (this->isOpInChannel(user))
			{
				this->operators.erase(it);
				std::cout << DELETE << "Erasing operator " << user->nickname << " from #" << this->name << RESET << std::endl;
			}
		}
		it++;
	}
	if (this->operators.empty() && !this->users.empty())
		this->allocNewOp(server);
	else if (this->operators.empty() && this->users.empty())
	{
		std::cout << DELETE << "Channel " << this->name << " is empty, erasing it from server..." << RESET << std::endl;
		server->channels.erase(server->getChannelByName(this->name));
		return 1;
	}
	return 0;
}

void	Channel::allocNewOp(Server * server)
{
	std::cout << YELLOW << "#" << this->name << " is empty, allocating a new operator..." << RESET << std::endl;
	User *	newOp = this->users.front();
	this->users.erase(this->users.begin());
	this->operators.push_back(newOp);
	std::cout << YELLOW << "Adding " << newOp->nickname << " as new channel operator." << RESET << std::endl;
	this->printChannelUsers(YELLOW);
	irc_names(this, newOp, server);
}

std::vector<User *>::iterator	Channel::getUserByNick(std::string nickname)
{
	std::vector<User *>::iterator	it = this->users.begin();
	std::vector<User *>::iterator	ite = this->users.end();

	while (it != ite)
	{
		if ((*it)->nickname == nickname)
			return it;
		it++;
	}

	it = this->operators.begin();
	ite = this->operators.end();

	while (it != ite)
	{
		if ((*it)->nickname == nickname)
			return it;
		it++;
	}
	return this->users.end();
}

void	Channel::opUser(User * user ,std::vector<User *>::iterator iterator, Server * server)
{
	std::vector<User *>::iterator	it = this->users.begin();
	std::vector<User *>::iterator	ite = this->users.end();

	if (this->isOpInChannel(user) == false)
	{
		std::cout << ERROR << "User " << user->nickname << " is not operator in channel # " << this->name << RESET << std::endl;
		return;
	}

	while (it != ite)
	{
		if (it == iterator)
		{
			this->operators.push_back((*it));
			this->users.erase(it);
			break;
		}
		it++;
	}
	if (it != ite)
	{
		irc_names(this, (*it), server);
		std::cout << YELLOW << "Operator " << user->nickname << " succesfully set " << (*iterator)->nickname << " as new channel operator in #" << this->name << RESET << std::endl;
	}
}

void	Channel::deOpUser(User * user ,std::vector<User *>::iterator iterator, Server * server)
{
	std::vector<User *>::iterator	it = this->operators.begin();
	std::vector<User *>::iterator	ite = this->operators.end();

	if (this->isOpInChannel(user) == false)
	{
		std::cout << ERROR << "User " << user->nickname << " is not operator in channel # " << this->name << RESET << std::endl;
		return;
	}
	if (this->isBestOp(user, iterator) == false)
	{
		std::cout << ERROR << "Operator " << user->nickname << " have less privileges than " << (*iterator)->nickname << " in channel #" << this->name << RESET << std::endl;
		return;
	}

	while (it != ite)
	{
		if (it == iterator)
		{
			this->users.push_back((*it));
			this->operators.erase(it);
			break;
		}
		it++;
	}
	if (it != ite)
	{
		irc_names(this, (*it), server);
		std::cout << YELLOW << "Operator " << user->nickname << " succesfully remove " << (*iterator)->nickname << " as channel operator in #" << this->name << RESET << std::endl;
	}
	if (this->operators.empty())
		this->allocNewOp(server);
}

bool	Channel::isBestOp(User * user, std::vector<User *>::iterator iterator)
{
	int	userPos = 0;
	int	contenderPos = 0;

	std::vector<User *>::iterator	it = this->operators.begin();
	std::vector<User *>::iterator	ite = this->operators.end();

	if (user == (*iterator))
		return true;

	while (it != ite && userPos == 0 && contenderPos == 0)
	{
		if ((*it) == user)
			userPos = 1;
		if (it == iterator)
			contenderPos = 1;
		it++;
	}
	if (userPos == 1 && contenderPos == 0)
		return true;
	return false;
}

bool	Channel::isModeI() const
{
	if (this->mode.find('i') != std::string::npos)
		return true;
	return false;
}

bool	Channel::isModeT() const
{
	if (this->mode.find('t') != std::string::npos)
		return true;
	return false;
}

bool	Channel::isModeK() const
{
	if (this->mode.find('k') != std::string::npos)
		return true;
	return false;
}

bool	Channel::isModeL() const
{
	if (this->mode.find('l') != std::string::npos)
		return true;
	return false;
}
