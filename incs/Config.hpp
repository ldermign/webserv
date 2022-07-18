/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 16:00:24 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/18 17:56:58 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "webserv.hpp"

class Config {

public:

//	recup info

	void	recupInfoServer( std::vector< std::string > args );

//	check info

	void	checkAll( const char *str ) {
		try {
			this->getParse().setNameFile(str);
			this->getParse().checkFileName();
			this->getParse().setFileVector();
			this->getParse().setArgsFile();
			this->getParse().checkFileAllTogether();
			this->getParse().checkAllDirectives();
			this->recupInfoServer(this->getParse().getArgs());
		}
		catch (const std::exception &e) {
			std::cout << e.what() << std::endl;
			exit (EXIT_FAILURE);
		}
	}
	void	getInfo( void ) {
		
		// Server instance;
		// _servers.push_back(instance);
	}

	//	ACCESSORS

	Parse						&getParse( void ) { return this->_config; }
	std::vector< Server >		&getVectorServers( void ) { return this->_servers; }

	//	CANONICAL FORM

	Config( void ){
		// Server instance;
		// _servers.push_back(instance);
	}
	//  : _servers(*new std::vector< Server * >) {};
	Config	&operator=( const Config &rhs ) {
		if (this != &rhs) {
		}
		return *this;
	}
	Config( const Config &src ) // : _servers(*new std::vector< Server * >) 
	{
		*this = src;
	}
	virtual	~Config( void ) {};

private:

	Parse						_config;
	std::vector< Server >		_servers; // reference ?


};

#endif