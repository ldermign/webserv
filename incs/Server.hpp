/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:27:06 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/19 16:50:21 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

// #include "Parse.hpp"
// #include "Server.hpp"
#include "webserv.hpp"
#include "Location.hpp"

class Server {
	
public:

//
	int							setServer( std::vector< std::string >::iterator it );
	int							setName( std::vector< std::string >::iterator it );
	int							setListen( std::vector< std::string >::iterator it );
	int							setClient( std::vector< std::string >::iterator it );

//	ACCESSORS

	Server						&getServer( void ) { return *this; }

	std::string					getHost( void ) { return this->_host; }
	int							getPort( void ) { return this->_port; }
	std::vector< std::string >	&getServerName( void ) { return this->_serverName; }
	bool						getDefaultServer( void ) { return this->_defaultServer; }
	std::vector< int >			&getErrorCode( void ) { return this->_errorCode; }
	size_t						getClient( void ) { return this->_clientMaxBodySize; }
	std::vector< Location >		&getLocation( void ) { return this->_location; }

//	CANONICAL FORM

	Server( void )
		: _host("0.0.0.0"), _port(80), _serverName(), _defaultServer(0),
		_errorCode(0), _clientMaxBodySize(50), _location() {};
	Server	&operator=( const Server &rhs ) {
		if (this == &rhs) 
			return *this;
		
		this->_host = rhs._host;
		this->_port = rhs._port;
		this->_serverName = rhs._serverName;
		this->_defaultServer = rhs._defaultServer;
		this->_errorCode = rhs._errorCode;
		this->_clientMaxBodySize = rhs._clientMaxBodySize;
		this->_location = rhs._location;
		
		return *this;
	}
	Server( const Server &src ) {
		*this = src;
	}
	virtual	~Server( void ) {};

private:

	std::string					_host;
	int							_port;
	std::vector< std::string >	_serverName;
	bool						_defaultServer;
	std::vector< int >			_errorCode;
	size_t						_clientMaxBodySize;
	std::vector< Location >		_location;
	
};


#endif