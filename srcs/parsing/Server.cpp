/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 16:57:06 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/18 18:05:48 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Location.hpp"
#include "Server.hpp"

int	Server::setName( std::vector< std::string >::iterator it ) {
	
	int ret = 0, i = 0;
	while (it[ret] != ";")
		ret++;

	*it++;
	while (i < ret) {
		
		this->_serverName.push_back(it->c_str());
		*it++;
		i++;
	}

	return ret;
}

int	Server::setListen( std::vector< std::string >::iterator it ) {
	
	int ret = 0;
	
	*it++;
	while (it[ret] != ";")
		ret++;

	this->_host = "0.0.0.0";
	this->_port = 80;
	this->_defaultServer = 0;

	{
		std::string tmp = it[0].c_str();
		if (tmp.find(".") != std::string::npos)
			this->_host = tmp;
		else if (tmp == "default_server")
			this->_defaultServer = 1;
		else
			this->_port = atoi(tmp.c_str());
	}

	if (ret > 1)
	{
		std::string tmp = it[1].c_str();
		if (tmp.find(".") != std::string::npos)
			this->_host = tmp;
		else if (tmp == "default_server")
			this->_defaultServer = 1;
		else
			this->_port = atoi(tmp.c_str());
	}

	if (ret == 3)
		this->_defaultServer = 1;

	return ret + 1;
}

int	Server::setClient( std::vector< std::string >::iterator it ) {
	
	*it++;

	std::string tmp = it->c_str();
	this->_clientMaxBodySize = atoi(tmp.c_str());

	return 3;
}

int	Server::setServer( std::vector< std::string >::iterator it) {

	int ret = 2;

	*it++;
	*it++;
	while (*it != "}") {
		if (*it == "server_name")
			ret = setName(it);
		else if (*it == "listen")
			ret = setListen(it);
		else if (*it == "client_max_body_size")
			ret = setClient(it);
		else if (*it == "location") {
			Location instance;
			_location.push_back(instance);
			ret = instance.setLocation(it);
		}
		for (int i = 0 ; i < ret ; i++)
			*it++;
		*it++;
	}
	return ret + 2;
}
