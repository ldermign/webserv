/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_config_file.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 13:27:38 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/05 16:02:21 by ldermign         ###   ########.fr       */
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

int	ft_server( char const *str ) {

	int i = 6;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '\0')
		return EXIT_SUCCESS;
	if (str[i] && str[i] != '{' && str[i] != '\0')
		return EXIT_FAILURE;
	i++;
	while (str[i] && (str[i] == ' ' || str[i]  == '\t'))
		i++;
	if (str[i] != '\0')
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int	ft_server_name( char const *str ) { (void)str;
	
	return EXIT_SUCCESS;
}

int	ft_listen( char const *str ) { (void)str;
	
	return EXIT_SUCCESS;
}

int	ft_root( char const *str ) { (void)str;
	
	return EXIT_SUCCESS;
}

int	ft_index( char const *str ) { (void)str;
	
	return EXIT_SUCCESS;
}

int	ft_get_methods( char const *str ) { (void)str;
	
	return EXIT_SUCCESS;
}

int	ft_client_max_body_size( char const *str ) { (void)str;
	
	return EXIT_SUCCESS;
}

int	ft_autoindex( char const *str ) { (void)str;
	
	return EXIT_SUCCESS;
}

int	ft_location( char const *str ) { (void)str;
	
	return EXIT_SUCCESS;
}

int	ft_cgi( char const *str ) { (void)str;
	
	return EXIT_SUCCESS;
}

int	ft_auth( char const *str ) { (void)str;
	
	return EXIT_SUCCESS;
}

int	ft_return( char const *str ) { (void)str;
	
	return EXIT_SUCCESS;
}

void	check_line_by_line( char const *str ) {

	static int line = 0;
	
	line++;
	if (*str == '\0' || *str == '#' || *str == '{' || *str == '}')
		return ;

	const t_dir whichDirective[] = {
		{"server_name ", ft_server_name},
		{"server", ft_server},
		{"listen ", ft_listen},
		{"root ", ft_root},
		{"index ", ft_index},
		{"get_methods ", ft_get_methods},
		{"client_max_body_size ", ft_client_max_body_size},
		{"autoindex ", ft_autoindex},
		{"location ", ft_location},
		{"cgi ", ft_cgi},
		{"auth ", ft_auth},
		{"return ", ft_return}
		};

	int i = 0;
	while (i < 12) {
		std::string tmp = static_cast< std::string >(str).substr(0, strlen(whichDirective[i].directive));
		if (whichDirective[i].directive == tmp) {
			if (whichDirective[i].f(str) == EXIT_FAILURE) {
				std::ostringstream add;
				add << line;
				std::string error_msg = "Error line " + add.str() + " for directive " + whichDirective[i].directive;
				throw std::string(error_msg);
			}
			return ;
		}
		i++;
	}


	{	//	error in line
		std::ostringstream add;
		add << line;
		std::string error_msg = "Error line " + add.str();
		throw std::string(error_msg);
	}


/*
**	Erreur sur la fin de la ligne
*/

	// if (*str != ';' && *str != '{' && *str != '}') {
	// 	std::ostringstream add;
	// 	add << line;
	// 	std::string error_msg = "Error line " + add.str();
	// 	std::cout << error_msg << std::endl;
	// 	// throw std::string(error_msg);
	// }
}

static int	check_file_all_together( char const *str ) {

	std::string line;
	std::ifstream tmp(str);
	int bracket = 0;
	int server = 0;

	do {
		std::getline(tmp, line);
		if (line.find('{') != std::string::npos)
			bracket++;
		if (line.find('}') != std::string::npos)
			bracket--;
		{
			int	j = 0;
			while (line[j] && (line[j] == ' ' || line[j] == '\t'))
				j++;
			if (line.find("server") != std::string::npos) {
				j += 6;
				if (!line[j] || (line[j] && (line[j] == ' ' || line[j] == '\t')))
					server++;
			}
		}
	} while (!tmp.eof());
	tmp.close();
	if (bracket != 0)
		return EXIT_FAILURE;
	return server;
}

int	check_parsing_file( char const *str ) {

	std::string line;
	std::ifstream tmp(str);
	
	int ret = check_file_all_together(str);
	if (ret == EXIT_FAILURE)
		throw ConfigurationFile::BadBracket();
	do {
		std::getline(tmp, line);
		int i = 0;
		while (line[i] == ' ' || line[i] == '\t')
			i++;
		try { check_line_by_line(&line[i]); }
		catch (const std::string &e) {
			tmp.close();
			std::cout << e << std::endl;
			exit (EXIT_FAILURE);
		}
	} while (!tmp.eof());
	tmp.close();
	return ret;
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
		int	nbr_server;
		check_file(str);
		nbr_server = check_parsing_file(str);
		ConfigurationFile	config[nbr_server];
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		exit (EXIT_FAILURE);
	}

	return ;
}