/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 19:12:29 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/04 22:30:00 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include <string>
#include <map>

class User;

class Channel
{
	private:
		std::string		_name;
		std::string		_topic;
		std::map<std::string, User>	_users;
	public:
		Channel();
		Channel(std::string name, std::string topic);
		~Channel();
		std::string		getName();
		std::string		getTopic();
		void			setName(std::string name);
		void			setTopic(std::string topic);
		void			addUser(User user);
		void			removeUser(User user);
};

#include "User.hpp"

#endif
