/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 16:01:12 by ldermign          #+#    #+#             */
/*   Updated: 2022/09/07 15:12:07 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Parse.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include <vector>

void	Config::checkAll( const char *str ) {
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
	this->recupInfoServer(this->getParse().getArgs());
}

void	Config::recupInfoServer( std::vector< std::string > args ) {
	
	std::vector< std::string >::iterator it;
	it = args.begin();

	while (it != args.end()) {
		
		if (*it == "server") {
			Server instance;
			instance.setServer(it, args.end());
			_servers.push_back(instance);
		}
		*it++;
	}
}