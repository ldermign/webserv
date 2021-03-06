/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:21:36 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/19 23:29:34 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Location.hpp"

int	Location::setMethods( std::vector< std::string >::iterator it ) {
	
	int ret = 1;
	
	*it++;
	while (*it != ";") {
		this->_methods.push_back(*it);
		*it++;
		ret++;
	}
	return ret + 1;
}

int	Location::setReturn( std::vector< std::string >::iterator it ) {
	
	*it++;
	std::string tmp = it->c_str();
	this->_returnCode = atoi(tmp.c_str());

	*it++;
	this->_returnPath = *it;

	return 4;
}


int	Location::setRoot( std::vector< std::string >::iterator it ) {
	
	*it++;

	this->_root = it->c_str();

	return 3;
}

int	Location::setIndex( std::vector< std::string >::iterator it ) {
	
	int ret = 1;
	
	*it++;
	while (*it != ";") {
		this->_index.push_back(*it);
		*it++;
		ret++;
	}

	return ret + 1;
}

int	Location::setCgi( std::vector< std::string >::iterator it ) {
	
	*it++;
	this->_cgiExtension = it->c_str();

	*it++;
	this->_cgiPath = it->c_str();

	return 4;
}

int	Location::setAutoindex( std::vector< std::string >::iterator it ) {
	
	*it++;

	if (*it == "on")
		this->_autoindex = 1;

	return 3;
}

int	Location::setDownload( std::vector< std::string >::iterator it ) {
	
	*it++;

	this->_download = it->c_str();

	return 3;
}

int	Location::setLocation( std::vector< std::string >::iterator it ) {
	
	int ret = 3, len = 0;
	
	*it++;
	this->_path = it->c_str();
	*it++;
	*it++;
	
	while (*it != "}") {

		// std::cout << *it << std::endl;
		if (*it == "get_methods")
			len = this->setMethods(it);
		else if (*it == "return")
			len = this->setReturn(it);
		else if (*it == "root")
			len = this->setRoot(it);
		else if (*it == "index")
			len = this->setIndex(it);
		else if (*it == "autoindex")
			len = this->setAutoindex(it);
		else if (*it == "cgi")
			len = this->setCgi(it);
		else if (*it == "download")
			len = this->setDownload(it);
		ret += len;
		for (int i = 0 ; i < len ; i++)
			*it++;
	}

	return ret + 1;
}