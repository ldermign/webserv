/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_config_file.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 13:27:38 by ldermign          #+#    #+#             */
/*   Updated: 2022/06/29 14:51:24 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "ConfigurationFile.hpp"
#include <fstream>
#include <exception>
#include <sys/stat.h>
#include <cstdlib>
#include <vector>
#include <sstream>

void	check_line_by_line( std::string::iterator str ) {

	static int line = 0;
	
	std::cout << *str << std::endl;
	if (*str != ';' && *str != '{' && *str != '}') {
		std::ostringstream add;
		add << line;
		std::string error_msg = "Error line " + add.str();
		std::cout << error_msg << std::endl;
		// throw std::string(error_msg);
	}
	line++;
}

void	check_parsing_file( char const *str ) {

	std::string line;
	std::ifstream tmp(str);
	
	do {
		std::getline(tmp, line);
		std::string::iterator it = line.begin();
		while (*it == ' ')
			it++;
		check_line_by_line(it);
	} while (!tmp.eof());
	tmp.close();
}

void	check_file( char const *str ) {

	std::ifstream tmp;

	tmp.open(str);
	if (tmp.fail())
		throw ConfigurationFile::BadFile();
	{
		bool empty = (tmp.get(), tmp.eof());
		if (empty) {
			tmp.close();
			throw ConfigurationFile::EmptyFile();
		}
	}
	{
		struct stat path_stat;
    	stat(str, &path_stat);
    	if (!S_ISREG(path_stat.st_mode))
			throw ConfigurationFile::FileIsDir();
	}
	tmp.close();
}

void	recup_config_file( char const *str ) { (void)str;

	try {
		check_file(str);
		check_parsing_file(str);
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		exit (EXIT_FAILURE);
	}

	return ;
}