/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.fr> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:42:13 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/06 14:31:39 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_irc.hpp"

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
		send(user.getFd(), "Quit server: ", 14, 0);
		close(user.getFd());
		user.setFd(-1);
	}
	else if (strcmp(buffer, "/help") == 0)
		displayHelp(user);
	else
		send(user.getFd(), "Unknown command\n/help for list of commands\n", 44, 0);
}

int main(int argc, char const* argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv [port] [password]"<<std::endl;
		return 0;
	}

	int								valread;
	char							buffer[BUFFSIZE + 1];
	std::map<std::string, Channel>	channels;

	Server	server(argv[1]);

	User user = init(server._newEntry, argv[2]);
	channels.insert(std::pair<std::string, Channel>("general", Channel("general", "general")));
	user.setChannel(channels["general"]);
	while (1)
	{
		bzero(buffer, BUFFSIZE);
		send(server._newEntry, "> ", 2, 0);
		valread = read(server._newEntry, buffer, BUFFSIZE);
		if (buffer[0] == '/')
		{
			buffer[valread - 1] = '\0';
			getorder(buffer, user, channels);
		}
		else
		{
			if (user.getNickname().empty())
				std::cout << user.getName() << " in " << user.getChannel().getName() << ": " << buffer;
			else
				std::cout << user.getNickname() << " in " << user.getChannel().getName() << ": " << buffer;
		}
		bzero(buffer, BUFFSIZE);
	}
	close(server._newEntry);
	return 0;
}
