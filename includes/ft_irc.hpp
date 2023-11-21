/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 14:04:04 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/21 10:49:30 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/poll.h>
#include <algorithm>
#include <map>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <iomanip>
#include <csignal>
#include <exception>
#include <stdexcept>
#include <ctime>
#include <sstream>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define GREEN   "\033[32m[SND_BUFFER]"      /* Green */
#define YELLOW  "\033[33m[STATUS] : "      /* Yellow */
#define COMMAND "\033[33m[COMMAND] > "
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m[COMMAND]"      /* Magenta */
#define CYAN    "\033[36m[RCV_BUFFER]"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define DIVIDER "\033[33m------------------------------------------------------------"

#define PRIVMSG	"\033[33m[COMMAND][PRIVMSG] > "
#define JOIN	"\033[33m[COMMAND][JOIN]"
#define INVITE	"\033[33m[COMMAND][INVITE]"
#define QUIT	"\033[33m[COMMAND][QUIT] > "
#define DELETE	"\033[31m[DELETE] : "
#define ERROR	"\033[31m[ERROR] : "

#define BUFFSIZE	1024
#define TRUE		1
#define FALSE		0

#define ERR_PASSWDMISMATCH		"464"

#include "Server.hpp"
#include "User.hpp"
#include "Channel.hpp"

//interceptor.cpp
void interpretor(char *message, int fd, Server * server);
User	*findUser(int fd, Server * server);
void	commands(char *message, User *user, Server *server);

//commands.cpp
void	join_rmInvit(Channel *channel, User *user);
void	irc_join(char *message, User *user, Server *server);
void	irc_privmsg(char *message, User *user, Server *server);
void	irc_part(char *message, User *user, Server *server);
void	irc_names(Channel *channel, User *user, Server *server);
void	irc_quit(char *message, User *user, Server *server);
void	irc_nick(char *message, User *user, Server *server);
void	irc_mode(char *message, User *user, Server *server);
void	irc_topic(char *message, User *user, Server *server);
void	irc_kick(char *message, User *user, Server *server);
void	irc_invite(char *message, User *user, Server *server);

//logs.cpp
void	send_log(int & fd, const char * message, Server * server);
void	read_log(int & fd, char * buffer, Server * server);

#endif
