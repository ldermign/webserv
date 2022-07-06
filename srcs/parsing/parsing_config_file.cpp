/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_config_file.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 13:27:38 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/06 15:26:14 by ldermign         ###   ########.fr       */
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

void	check_server_by_server( char const *str ) {

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

void	check_parsing_file( char const *str ) {

	std::string line;
	std::ifstream tmp(str);

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
}

void	recup_config_file( char const *str, std::vector< ConfigurationFile > config ) {

	int	nbr_server = 0;
	int	i = -1;
	int ret = 0;

	try {
		nbr_server = check_file_all_together(str);
		ConfigurationFile	config[nbr_server];
		while (i < nbr_server) {
			ret += check_server_by_server(str, config[i]);
			i++;
		}
	}
	catch (std::exception &e) {
		std::cout << e.what() << std::endl;
		exit (EXIT_FAILURE);
	}

	return ;
}