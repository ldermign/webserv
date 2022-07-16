/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 16:00:24 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/16 13:06:57 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "webserv.hpp"

class Config {

public:

	void	checkAll( const char *str ) {
		try {
			this->getParse().setNameFile(str);
			this->getParse().checkFileName();
			this->getParse().setFileVector();
			this->getParse().setArgsFile();
			this->getParse().checkFileAllTogether();
			this->getParse().checkAllDirectives();
			
		}
		catch (const std::exception &e) {
			std::cout << e.what() << std::endl;
			exit (EXIT_FAILURE);
		}
	}
	void	getInfo( void ) {
		
		int i = 0;
		while (i < this->getParse().getNbrServer()) {
			this->_servers = new *Server();
			i++;
		}
	}

	//	ACCESSORS

	Parse						&getParse( void ) { return this->_config; }
	std::vector< Server * >		&getServers( void ) { return this->_servers; }

	//	CANONICAL FORM

	Config( void ) {};
	Config	&operator=( const Config &rhs ) {
		if (this != &rhs) {
		}
		return *this;
	}
	Config( const Config &src ) {
		*this = src;
	}
	virtual	~Config( void ) {};

private:

	Parse						_config;
	std::vector< Server >		*_servers; // reference ?


};

#endif