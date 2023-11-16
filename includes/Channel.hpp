/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cparras <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:34:14 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/16 17:00:59 by cparras          ###   ########.fr       */
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
		std::vector<User *>		users;
		std::vector<User *>		operators;

		bool	isInChannel(User *user);
		bool	isOpInChannel(User *user);
		void	printChannelUsers() const;
		void	channelSendLoop(std::string message, int & sFd, Server * server, int sendToHim);
		void	deleteChannelUser(User * user);

		Channel(std::string name);
		~Channel();
};

#endif
