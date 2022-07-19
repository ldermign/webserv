/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 16:01:12 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/19 18:38:40 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "Parse.hpp"
#include "Server.hpp"
#include "Config.hpp"
#include <vector>

void	Config::recupInfoServer( std::vector< std::string > args ) {
	
	std::vector< std::string >::iterator it;
	it = args.begin();

	while (it != args.end()) {
		
		if (*it == "server") {
			Server instance;
			instance.setServer(it);
			_servers.push_back(instance);
		}
		*it++;
	}

}