/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 15:15:56 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/11 17:35:40 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

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
		std::string getName() const;
		std::string getNickname() const;
		bool		getHasNickname() const;
		int			getFd() const;
		Channel		getChannel() const;
		void		setName(std::string name);
		void		setNickname(std::string nickname);
		void		setFd(int fd);
		void		setHasNickname(bool hasNickname);
		void		setChannel(Channel channel);
		void		sendMessage(std::string message) const;
};

User	init(int new_socket, const char *password);

#endif
