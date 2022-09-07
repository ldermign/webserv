/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 16:00:24 by ldermign          #+#    #+#             */
/*   Updated: 2022/09/07 15:13:11 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "webserv.hpp"

class Config {

public:

	void	recupInfoServer( std::vector< std::string > args );
	void	checkAll( const char *str );

	//	ACCESSORS

	Parse						&getParse( void ) { return this->_config; }
	std::vector< Server >		&getVectorServers( void ) { return this->_servers; }

	//	CANONICAL FORM

	Config( void ) : _config(), _servers() {}
	Config	&operator=( const Config &rhs ) {
		if (this == &rhs) 
			return *this;

		this->_config = rhs._config;
		this->_servers = rhs._servers;

		return *this;
	}
	Config( const Config &src )	{
		*this = src;
	}
	virtual	~Config( void ) {};

private:

	Parse						_config;
	std::vector< Server >		_servers; // reference ?


};

#endif