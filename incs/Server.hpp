/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:27:06 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/06 14:17:31 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <vector>

class Server {
	
public:

//	ACCESSORS

	std::string	getRootFile( void );
	int			getPort( void );
	std::string	getServerName( void );

//	CANONICAL FORM

	Server( void ) {};
	Server	&operator=( const Server &rhs ) {
		if (this != &rhs) {
		}
		return *this;
	}
	Server( const Server &src ) {
		*this = src;
	}
	virtual	~Server( void ) {};

private:


	std::vector< std::string >	_listen;
	std::vector< std::string >	_serverName;
	// std::map< int >				_clientMaxBodySize; revoir caaa la
	std::vector< std::string >	_methods;
	std::vector< std::string >	_index;
	std::string					_root;
	std::string					_cgi;
	bool						_autoIndex;

	// int			_port;
	
	// la location qui doit avoir une classe dedans;
	// voir comment faire le port
	
};


#endif