/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 15:42:13 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/16 18:49:33 by rrodor           ###   ########.fr       */
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
	std::cout << "buffer: |" << buffer << "|" << std::endl;
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
	int								rc = 1;
	int								nfds = 1;
	int								new_sd = 0;
	int								close_conn;
	int								current_size;
	char							buffer[BUFFSIZE + 1];
	std::map<std::string, Channel>	channels;
	std::map<int, User>		users;
	struct pollfd					fds[200];
	int								i = 0;
	//User							user;

	Server	server(argv[1]);
	Channel general("general", "general");
	User user(i, general);

	/*User user = init(server._newEntry, argv[2]);
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
			{
				std::cout << user.getName() << " in " << user.getChannel().getName() << ": " << buffer;
				user.sendMessage(buffer);
			}
			else
			{
				std::cout << user.getNickname() << " in " << user.getChannel().getName() << ": " << buffer;
				user.sendMessage(buffer);
			}
		}
		bzero(buffer, BUFFSIZE);
	}*/
	//std::cout << "server.getServerSocket(): " << server.getServer() << std::endl;

	//Channel general("general", "general");
	memset(fds, 0 , sizeof(fds));
	fds[0].fd = server.getServer();
	fds[0].events = POLLIN;
	do
	{
		//printf("Waiting on poll()...\n");
		rc = poll(fds, nfds, -1);
		current_size = nfds;
		for (int i = 0; i < current_size; i++)
		{
			if(fds[i].revents == 0)
				continue;
			new_sd = 0;
			//std::cout << "test" <<std::endl;
			if (fds[i].fd == server.getServer())
			{
				//std::cout << "new_sd = " << new_sd << std::endl;
				while (new_sd != -1)
				{
					new_sd = accept(server.getServer(), NULL, NULL);
					std::cout << "new user connected: " << new_sd << std::endl;
					if (new_sd != -1)
					{
						fds[nfds].fd = new_sd;
						fds[nfds].events = POLLIN;
						nfds++;
						User user = init(fds[nfds - 1].fd, argv[2]);
						user.setChannel(general);
						users.insert(std::pair<int, User>(fds[nfds - 1].fd, user));
						send(fds[nfds - 1].fd, "> ", 2, 0);
					}
				}
			}
			else
			{
				close_conn = FALSE;
				send(fds[i].fd, "> ", 2, 0);
				rc = recv(fds[i].fd, buffer, sizeof(buffer), 0);
				if (rc == 0)
				{
					printf("  Connection closed\n");
					close_conn = TRUE;
					break;
				}
				buffer[rc] = '\0';
				user = users[fds[i].fd];
				//std::cout << "user: " << user.getName() << std::endl;
				if (buffer[0] == '/')
				{
					buffer[rc - 1] = '\0';
					getorder(buffer, user, channels);
					send(fds[nfds - 1].fd, "> ", 2, 0);
				}
				else
				{
					user.sendMessage(buffer);
					/*for (std::map<std::string, User>::iterator it = general.getUsers().begin(); it != general.getUsers().end(); it++)
					{
						std::cout << "name " << it->first << std::endl;
					}*/
					std::cout << user.getName() << " in " << user.getChannel().getName() << ": " << buffer;
				}
				if (close_conn)
				{
					close(fds[i].fd);
					fds[i].fd = -1;
				}
			}
		}
	} while (1);
	close(server._newEntry);
	return 0;
}
