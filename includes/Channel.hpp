/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrodor <rrodor@student.42perpignan.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/04 19:12:29 by rrodor            #+#    #+#             */
/*   Updated: 2023/10/11 17:36:39 by rrodor           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

class User;

class Channel
{
	private:
		std::string			_name;
		std::string			_topic;
	public:
		Channel();
		Channel(std::string name, std::string topic);
		~Channel();
		std::string					getName() const;
		std::string					getTopic() const;
		void			setName(std::string name);
		void			setTopic(std::string topic);
};

std::ostream &	operator<<(std::ostream & o, Channel const &rSym);

#endif
