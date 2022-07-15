/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 13:27:06 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/15 15:14:02 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "ConfigurationFile.hpp"
#include "Server.hpp"

class Server {
	
public:

	void	checkAll( const char *str ) {
		try {
			this->getConfigurationFile().setNameFile(str);
			this->getConfigurationFile().checkFileName();
			this->getConfigurationFile().setFileVector();
			this->getConfigurationFile().setArgsFile();
			this->getConfigurationFile().checkFileAllTogether();
			this->getConfigurationFile().checkAllDirectives();
			
		}
		catch (const std::exception &e) {
			std::cout << e.what() << std::endl;
			exit (EXIT_FAILURE);
		}
	}

	ConfigurationFile	&getConfigurationFile( void ) { return this->_config; }

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
	

	ConfigurationFile _config;
	
};


#endif