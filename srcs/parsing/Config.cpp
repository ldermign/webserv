/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 16:01:12 by ldermign          #+#    #+#             */
/*   Updated: 2022/09/12 10:25:41 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Parse.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include <vector>

void	Config::checkAll( const char *str ) {

	this->getParse().setNameFile(str);
	this->getParse().checkFileName();
	this->getParse().setFileVector();
	this->getParse().setArgsFile();
	this->getParse().checkFileAllTogether();
	this->getParse().checkAllDirectives();
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
