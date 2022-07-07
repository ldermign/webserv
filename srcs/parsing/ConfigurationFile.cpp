/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 14:22:32 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/07 15:51:02 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigurationFile.hpp"

void	ConfigurationFile::checkFileAllTogether( void ) {

	std::vector< std::string >::iterator	it;	// declare an iterator to a vector of strings
	int bracket = 0;
	int server = 0;

	for (it = this->_file.begin() ; it < this->_file.end() ; *it++) {
		if ((*it).find('{') != std::string::npos)
			bracket++;
		if ((*it).find('}') != std::string::npos)
			bracket--;
		if (this->directiveServer((*it).c_str()) == EXIT_SUCCESS)
			server++;
	}

	if (bracket != 0) {
		throw ConfigurationFile::BadBracket();
		return ;
	}
	this->setNbrServer(server);
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

void	ConfigurationFile::setFileVector( void ) {
	
	std::string line;
	std::ifstream tmp(this->getNameFile());

	do {
		std::getline(tmp, line);
		this->_file.push_back(line);
	} while (!tmp.eof());
	tmp.close();
}

void	ConfigurationFile::checkNothingOut( void ) {

	std::vector< std::string >::iterator	it;	// declare an iterator to a vector of strings
	int bracket = 0;
	int server = 0;

	for (it = this->_file.begin() ; it < this->_file.end() ; *it++) {
	
		if (this->directiveServer((*it).c_str()) == EXIT_SUCCESS)
			server++;
		
		if ((*it).find('{') != std::string::npos)
			bracket++;
		if ((*it).find('}') != std::string::npos)
			bracket--;
		if ((this->directiveServer((*it).c_str()) == EXIT_FAILURE && (*it)[0] != '\0' && (*it).find('}') == std::string::npos && bracket == 0)
			|| (server == 0 && this->directiveServer((*it).c_str()) == EXIT_FAILURE && (*it)[0] != '\0')) {
			std::cout << *it << " brackets = " << bracket << std::endl;
			throw ConfigurationFile::BlockServer();
		}

	std::cout << *it << std::endl;

		std::string tmp = it->c_str();
		int i = 0;
		while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t'))
			i++;

		if (this->directiveServer(&tmp[i]) == EXIT_FAILURE
			&& this->noDirective(&tmp[i]) == EXIT_FAILURE) {
			std::string error_msg = "Error line [ ~~~ " + *it + " ~~~ ]";
			throw ConfigurationFile::WrongInfo();
			std::cout << *it << std::endl;
		}
	}
}

/*	Check directives */

int	ConfigurationFile::noDirective( std::string str ) {

	if (str.find("server_name ") == std::string::npos
		&& str.find("root ") == std::string::npos
		&& str.find("listen ") == std::string::npos
		&& str.find("index ") == std::string::npos
		&& str.find("get_methods ") == std::string::npos
		&& str.find("auth ") == std::string::npos
		&& str.find("client_max_body_size ")  == std::string::npos
		&& str.find("autoindex ") == std::string::npos
		&& str.find("location ") == std::string::npos
		&& str.find("cgi ") == std::string::npos
		&& str.find("return ") == std::string::npos
		&& str.find(" {") == std::string::npos 
		&& str.find(" }") == std::string::npos
		&& str[0] != '{'
		&& str[0] != '}'
		&& str[0] != '#'
		&& str[0] != '\0')
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
