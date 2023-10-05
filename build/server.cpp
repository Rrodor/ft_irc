/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:42:13 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/05 19:13:56 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <vector>
#include "../includes/Channel.hpp"
#include "../includes/User.hpp"

#define BUFFSIZE 1024

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
	int								server_fd, new_socket, valread;
	struct sockaddr_in				address;
	int								opt = 1;
	int								addrlen = sizeof(address);
	char							buffer[BUFFSIZE + 1];
	std::map<std::string, Channel>	channels;

	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv [port] [password]"<<std::endl;
		return 0;
	}
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr("127.0.0.1");
	address.sin_port = htons(std::atoi(argv[1]));

	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	User user = init(new_socket, argv[2]);
	channels.insert(std::pair<std::string, Channel>("general", Channel("general", "general")));
	user.setChannel(channels["general"]);
	while (1)
	{
		bzero(buffer, BUFFSIZE);
		send(new_socket, "> ", 2, 0);
		valread = read(new_socket, buffer, BUFFSIZE);
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
	close(new_socket);
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
