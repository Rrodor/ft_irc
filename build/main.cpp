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

int main(int argc, char const* argv[])
{
	if (argc != 3)
	{
		std::cout << "Usage: ./ircserv [port] [password]"<<std::endl;
		return 0;
	}

	std::string		message;
	char			buffer[BUFFSIZE + 1];
	int				rc = 1, \
					i = 0, \
					fdsId = 1, \
					newSd = 0, \
					close_conn = 0;

	Server			*server;
	try
	{
		server = new Server(argv[1]);
	}
	catch(const std::exception& e)
	{
		std::cerr << "[ERROR] : Error during server creation : " << std::endl;
		std::cerr <<  e.what() << std::endl;
		delete server;
	}

	while (1)
	{
		std::cout << YELLOW << "[STATUS] : Waiting incoming connection ( poll() )..." << RESET << std::endl;
		rc = poll(server->fds, fdsId, -1);
		server->current_size = fdsId;
		for (int i = 0; i < server->current_size; i++)
		{
			if(server->fds[i].revents == 0)
				continue;
			newSd = 0;
			if (server->fds[i].fd == server->getServerSocket())
			{
				while (newSd != -1)
				{
					newSd = accept(server->getServerSocket(), NULL, NULL);
					if (newSd != -1)
					{
						std::cout << GREEN << "[STATUS] : New user connected with id " << newSd << "." << RESET << std::endl;
						if (server->newUser(newSd, argv[2], fdsId))
							delete server;
						fdsId++;
					}
				}
			}
			else if (server->fds[i].fd == -1)
				break;
			else
			{
				close_conn = FALSE;
				send(server->fds[i].fd, "> ", 2, 0);
				rc = recv(server->fds[i].fd, buffer, sizeof(buffer), 0);
				if (rc == 0)
				{
					printf("  Connection closed\n");
					close_conn = TRUE;
					break;
				}
				buffer[rc] = '\0';
				if (buffer[0] == '/')
				{
					buffer[rc - 1] = '\0';
					getorder(buffer, server->getUserByFd(server->fds[i].fd), server);
				}
				else
				{
					message = "[" + server->getUserByFd(server->fds[i].fd)->getName() + "] " + buffer;
					server->getUserByFd(server->fds[i].fd)->sendMessage(message, server->fds[i].fd, server->getUserByFd(server->fds[i].fd)->getChannel());
					
					std::cout << CYAN << "[MESSAGE][" << server->getUserByFd(server->fds[i].fd)->getName();
					std::cout << "][" << server->getUserByFd(server->fds[i].fd)->getFd() << "] in ";
					std::cout << server->getUserByFd(server->fds[i].fd)->getChannel()->getName();
					std::cout << " : " << buffer << RESET;
				}
				if (close_conn)
				{
					close(server->fds[i].fd);
					server->fds[i].fd = -1;
				}
			}
		}
	}
	std::cout << "END OF PROGRAM" << std::endl;
	return 0;
}
