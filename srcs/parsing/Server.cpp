/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/18 16:57:06 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/20 14:01:21 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Location.hpp"
#include "Server.hpp"

int	Server::setName( std::vector< std::string >::iterator it ) {
	
	int ret = 0;
	while (it[ret] != ";")
		ret++;

	*it++;
	while (*it != ";") {
		
		this->_serverName.push_back(it->c_str());
		*it++;
	}

	return ret + 1;
}

int	Server::setListen( std::vector< std::string >::iterator it ) {
	
	int ret = 0;
	
	*it++;
	while (it[ret] != ";")
		ret++;

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

	return ret + 2;
}

int	Server::setClient( std::vector< std::string >::iterator it ) {
	
	*it++;

	std::string tmp = it->c_str();
	this->_clientMaxBodySize = atoi(tmp.c_str());

	return 3;
}

int	Server::setError( std::vector< std::string >::iterator it ) {

	int ret = 0;
	
	*it++;
	while (it[ret] != ";")
		ret++;

	while (*it != ";") {
	
		std::string str = it->c_str();
		if (!isdigit(str[0]))
			break ;
		this->getErrorCode().push_back(atoi(str.c_str()));
		*it++;
	}

	this->_errorPath = it->c_str();
	
	return ret + 2;
}

int	Server::setServer( std::vector< std::string >::iterator it, std::vector< std::string >::iterator last ) {

	int ret = 2;

	*it++;
	*it++;

	while (it != last && *it != "}") {

		if (*it == "server_name")
			ret = setName(it);
		else if (*it == "listen")
			ret = setListen(it);
		else if (*it == "client_max_body_size")
			ret = setClient(it);
		else if (*it == "error_page")
			ret = setError(it);
		else if (*it == "location") {
			Location instance;
			ret = instance.setLocation(it);
			_location.push_back(instance);
		}
		for (int i = 0 ; it != last && i < ret ; i++)
			*it++;
	}
	return ret + 2;
}
