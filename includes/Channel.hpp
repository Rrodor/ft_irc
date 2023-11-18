/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babreton <babreton@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:34:14 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/18 17:10:05 by babreton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _CHANNEL_HPP_
# define _CHANNEL_HPP_

#include "ft_irc.hpp"

class Channel
{
	public:
		std::string				name;
		std::string				topic;
		std::string				lastTopicUpdate;
		std::string				mode;
		std::vector<User *>		users;
		std::vector<User *>		operators;
		std::vector<User *>		invitedUsers;

		bool	isInChannel(User *user);
		bool	isOpInChannel(User *user);
		void	printChannelUsers(std::string logType) const;
		void	channelSendLoop(std::string message, int & sFd, Server * server, int sendToHim);
		int		deleteChannelUser(std::vector<User *>::iterator iterator, Server * server);
		int		deleteChannelUser(User * user, Server * server);
		void	allocNewOp(Server * server);
		std::vector<User *>::iterator	getUserByNick(std::string nickname);

		Channel(std::string name);
		~Channel();
};

#endif
