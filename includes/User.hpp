/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:15:56 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/05 15:49:48 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include <string>
#include "Channel.hpp"

class Channel;

class User
{
	private:
		int					_fd;
		std::string 		_name;
		std::string 		_nickname;
		bool				_hasNickname;
		Channel				_channel;
	public:
		User(int fd);
		~User();
		std::string getName();
		std::string getNickname();
		bool		getHasNickname();
		int			getFd();
		Channel		getChannel();
		void		setName(std::string name);
		void		setNickname(std::string nickname);
		void		setFd(int fd);
		void		setHasNickname(bool hasNickname);
		void		setChannel(Channel channel);
};

User	init(int new_socket, const char *password);

//#include "Channel.hpp"

#endif
