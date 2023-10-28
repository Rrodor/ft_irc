/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cparras <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 16:17:50 by cparras           #+#    #+#             */
/*   Updated: 2023/10/28 17:40:59 by cparras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

std::string     trimName(std::string name, bool flag)
{
    int j = 0;

    for (int i = 0; i < name.length(); i++)
    {
        if (name[i] == ' ' || name[i] == '\t')
            j++;
        else
            break ;
    }
    name.erase(0, j);
    for (int i = name.length() - 1; i >= 0; i--)
    {
        if (name[i] == ' ' || name[i] == '\t')
            name[i] = '\0';
        else
            break ;
    }
    if (flag == 1)
        return name;
    for (int i = 0; i < name.length(); i++)
    {
        if (name[i] == ' ' || name[i] == '\t')
            name[i] = '_';
    }
    return name;
}

bool	checkEmptyName(char *src)
{
	int i = 0;

	if (!isprint(src[0]))
		return	false;
	while (src[i])
	{
		if (src[i] != ' ' && src[i] != '\t' && src[i] != '\n' && src[i] != '\0')
			return true;
		i++;
	}
	return false;
}

void	connectToClient(int fd, std::string password)
{
	char	buffer[BUFFSIZE + 1];
	int		valread;
	bool	connected = false;

	while (!connected)
	{
		send(fd, "Enter the password: ", 21, 0);
		valread = read(fd, buffer, BUFFSIZE);
		buffer[valread - 1] = '\0';
		if (password.compare(buffer) == 0)
		{
			send(fd, "Welcome to the server\n", 23, 0);
			connected = true;
			bzero(buffer, BUFFSIZE);
		}
		else
		{
			bzero(buffer, BUFFSIZE);
			send(fd, "Wrong password.\n", 17, 0);
		}
	}
}

void	displayHelp(User * user)
{
	send(user->getFd(), "List of commands:\n", 19, 0);
	send(user->getFd(), "/nick: change your nickname\n", 29, 0);
	send(user->getFd(), "/quit: quit the server\n", 24, 0);
	send(user->getFd(), "/channel -[flag]: depends on flag, for more details type /channel --help\n", 74, 0);
}

void	displayChannelFlags(User * user)
{
	send(user->getFd(), "List of flags:\n", 15, 0);
	send(user->getFd(), "\t-c: display current channel\n", 30, 0);	
	send(user->getFd(), "\t-l: display list of channels\n", 30, 0);
	send(user->getFd(), "\t-s: change current channel\n", 29, 0);
}

std::string newChannelName(std::string buffer)
{
    char *str = new char[buffer.length() + 1];
    std::strcpy(str, buffer.c_str());
    char *tmp = strtok(str, " \t");

    while (tmp != NULL)
    {
        str = tmp;
        tmp = strtok(NULL, " \t");
    }
    std::string result = str;
    return result;
}

bool	flagsChecker(std::string buffer)
{
	int	i = buffer.size();
	int j = 0;
	std::string	channelName;

	if (buffer[i - 1] == 's' && buffer[i - 2] == '-')
		return true;
	else
		return false;
}

bool	getorder(char* buffer, User * user, Server * server)
{
	int	valread;
	std::string str;
	std::cout << CYAN << "[COMMAND][" << user->getName() << "][" << user->getFd() << "] : enter command " << buffer << RESET << std::endl;
	if (strcmp(buffer, "/nick") == 0)
	{
		send(user->getFd(), "Enter a nickname: ", 19, 0);
		bzero(buffer, BUFFSIZE);
		valread = read(user->getFd(), buffer, BUFFSIZE);
		if (valread == 1)
			user->setHasNickname(false);
		else
		{
			buffer[strlen(buffer) - 1] = '\0';
			user->setNickname(buffer);
			bzero(buffer, BUFFSIZE);
		}
	}
	else if (strcmp(buffer, "/channel -c") == 0)
	{
		str = "current channel is: " + user->getChannel()->getName() + "\n";
		send(user->getFd(), str.c_str(), str.length(), 0);
	}
	else if (strcmp(buffer, "/channel -l") == 0)
	{
		str = "List of channels:\n";
		for (std::map<std::string, Channel *>::iterator it = server->getChannelsList().begin(); it != server->getChannelsList().end(); ++it)
			str += "\t-" + it->first + "\n";
		send(user->getFd(), str.c_str(), str.length(), 0);
	}
	else if (strncmp(buffer, "/channel -s", 11) == 0)
	{
		if (flagsChecker(buffer) == false)
		{
			std::string name = newChannelName(buffer);
			server->changeUserChannel(name, name, user);
			return 1;
		}
		else 
		{
			bzero(buffer, BUFFSIZE);
			send(user->getFd(), "Enter a channel name: ", 22, 0);
			valread = read(user->getFd(), buffer, BUFFSIZE);
			buffer[valread - 1] = '\0';
			std::string str = buffer;
			server->changeUserChannel(str, str, user);
		}
	}
	else if (strcmp(buffer, "/quit") == 0)
	{
		server->deleteUser(user->getFd(), user->getName(), user->getFdsId());
		return 0;
	}
	else if (strcmp(buffer, "/rootlist") == 0)
		server->printConnectedUsers();
	else if (strcmp(buffer, "/help") == 0)
		displayHelp(user);
	else if (strcmp(buffer, "/channel --help") == 0)
		displayChannelFlags(user);
	else
		send(user->getFd(), "Unknown command\n/help for list of commands\n", 44, 0);
	send(user->getFd(), "> ", 2, 0);
	return 1;
}