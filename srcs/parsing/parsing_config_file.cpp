/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_config_file.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 13:27:38 by ldermign          #+#    #+#             */
/*   Updated: 2022/06/27 14:05:48 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "ConfigurationFile.hpp"
#include <fstream>
#include <exception>

void	check_file( char const *str ) {

	std::ifstream tmp;

	tmp.open(str);
	if (tmp.fail())
		throw ConfigurationFile::BadFile();
	tmp.close();
}

void	recup_config_file( char const *str ) { (void)str;

	try {
		check_file(str);
		std::fstream tmp;
		tmp.open(str);
		
		tmp.close();
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		return ;
	}

	return ;
}