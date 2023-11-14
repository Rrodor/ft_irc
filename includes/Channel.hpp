/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 18:34:14 by rrodor            #+#    #+#             */
/*   Updated: 2023/11/13 20:01:52 by rrodor           ###   ########.fr       */
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
		std::vector<User *>		users;
		std::vector<User *>		operators;

		bool	isInChannel(User *user);
		bool	isOpInChannel(User *user);

		Channel(std::string name);
		~Channel();
};

#endif
