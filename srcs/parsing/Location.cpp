/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:21:36 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/18 18:04:27 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Location.hpp"


int	Location::setReturn( std::vector< std::string >::iterator it ) {
	
	*it++;

	std::string tmp = it->c_str();


	return 3;
}


int	Location::setLocation( std::vector< std::string >::iterator it ) {
	
	int ret = 3, len = 0;
	
	*it++;
	this->_path = it->c_str();
	*it++;
	
	while (*it != "}") {

		// if (*it == "get_methods")
			// len = this->dirGetMethods(it);
		if (*it == "return")
			len = this->setReturn(it);
		// else if (*it == "root")
		// 	len = this->dirRoot(it);
		// else if (*it == "index")
		// 	len = this->dirIndex(it);
		// else if (*it == "autoindex")
		// 	len = this->dirAutoindex(it);
		// else if (*it == "cgi")
		// 	len = this->dirCgi(it);
		// else if (*it == "download")
		// 	len = this->dirDownload(it);
		ret += len;
		for (int i = 0 ; i < len ; i++)
			*it++;
	}

	return ret + 1;
}