/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 14:22:32 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/06 15:59:05 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigurationFile.hpp"

int	ConfigurationFile::checkFileAllTogether( char const *str ) {

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
			if (this->directiveServer(line.c_str()) == EXIT_SUCCESS)
				server++;
		}
	} while (!tmp.eof());
	tmp.close();
	if (bracket != 0) {
		throw ConfigurationFile::BadBracket();
		return EXIT_FAILURE;
	}
	this->setNbrServer(server);
	return EXIT_SUCCESS;
}

void	ConfigurationFile::checkFileName( void ) {

	std::ifstream tmp;

	tmp.open(this->getNameFile());
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
    	stat(this->getNameFile(), &path_stat);
    	if (!S_ISREG(path_stat.st_mode))
			throw ConfigurationFile::FileIsDir();
	}
	tmp.close();
}

int	ConfigurationFile::directiveServer( char const *str ) {

	int i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (static_cast< std::string >(&str[i]).find("server") != std::string::npos)
		i += 6;
	else
		return EXIT_FAILURE;
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

/*
**	CANONICAL FORM
*/

ConfigurationFile	&ConfigurationFile::operator=( const ConfigurationFile &rhs ) {

	if (this != &rhs) {
		
	}
	
	return *this;
}

ConfigurationFile::ConfigurationFile( const ConfigurationFile &src ) {
	
	*this = src;
}
