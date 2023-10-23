/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cparras <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 16:17:50 by cparras           #+#    #+#             */
/*   Updated: 2023/10/23 14:17:28 by cparras          ###   ########.fr       */
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

void	displayHelp(User &user)
{
	send(user.getFd(), "List of commands:\n", 19, 0);
	send(user.getFd(), "/nick: change your nickname\n", 29, 0);
	send(user.getFd(), "/quit: quit the server\n", 24, 0);
}

void	getorder(char* buffer, User &user, std::map<std::string, Channel> &channels)
{
	int	valread;
	std::string str;
	std::cout << CYAN << "[COMMAND][" << user.getName() << "][" << user.getFd() << "] : enter command " << buffer << RESET << std::endl;
	if (strcmp(buffer, "/nick") == 0)
	{
		send(user.getFd(), "Enter a nickname: ", 19, 0);
		bzero(buffer, BUFFSIZE);
		valread = read(user.getFd(), buffer, BUFFSIZE);
		if (valread == 1)
			user.setHasNickname(false);
		else
		{
			buffer[strlen(buffer) - 1] = '\0';
			user.setNickname(buffer);
			bzero(buffer, BUFFSIZE);
		}
	}
	else if (strcmp(buffer, "/channel -c") == 0)
	{
		str = "current channel is: " + user.getChannel().getName() + "\n";
		send(user.getFd(), str.c_str(), str.length(), 0);
	}
	else if (strcmp(buffer, "/channel -l") == 0)
	{
		str = "List of channels:\n";
		for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
			str += "\t-" + it->first + "\n";
		send(user.getFd(), str.c_str(), str.length(), 0);
	}
	else if (strcmp(buffer, "/channel -s") == 0)
	{
		bzero(buffer, BUFFSIZE);
		send(user.getFd(), "Enter a channel name: ", 22, 0);
		valread = read(user.getFd(), buffer, BUFFSIZE);
		buffer[valread - 1] = '\0';
		str = buffer;
		user.setChannel(channels[str]);
		if (channels[str].getName() == "default")
		{
			Channel newChannel = Channel(str, str);
			channels.insert(std::pair<std::string, Channel>(str, newChannel));
			user.setChannel(newChannel);
		}
		else
			user.setChannel(channels[str]);
	}
	else if (strcmp(buffer, "/quit") == 0)
	{
		user.deleteUser();
		return;
	}
	else if (strcmp(buffer, "/help") == 0)
		displayHelp(user);
	else
		send(user.getFd(), "Unknown command\n/help for list of commands\n", 44, 0);
	send(user.getFd(), "> ", 2, 0);
}
