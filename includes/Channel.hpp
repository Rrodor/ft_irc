/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cparras <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 19:12:29 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/05 18:04:47 by cparras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

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

#endif
