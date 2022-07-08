/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 14:22:32 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/08 14:47:34 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
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
		if (this->isDirectiveServer((*it).c_str()) == EXIT_SUCCESS)
			server++;
	}

	this->setNbrServer(server);
	if (bracket != 0)
		throw ConfigurationFile::BadBracket();
	else if (server == 0)
		throw ConfigurationFile::NbrServer();
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

int	ConfigurationFile::isDirectiveServer( char const *str ) {

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
	
		if (this->isDirectiveServer((*it).c_str()) == EXIT_SUCCESS)
			server++;
		
		if ((*it).find('{') != std::string::npos)
			bracket++;
		if ((*it).find('}') != std::string::npos)
			bracket--;
		if ((this->isDirectiveServer((*it).c_str()) == EXIT_FAILURE && (*it)[0] != '\0' && (*it).find('}') == std::string::npos && bracket == 0)
			|| (server == 0 && this->isDirectiveServer((*it).c_str()) == EXIT_FAILURE && (*it)[0] != '\0')) {
			std::cout << *it << " brackets = " << bracket << std::endl;
			throw ConfigurationFile::BlockServer();
		}

	// std::cout << *it << std::endl;

		std::string tmp = it->c_str();
		int i = 0;
		while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t'))
			i++;

		if (this->isDirectiveServer(&tmp[i]) == EXIT_FAILURE
			&& this->noDirective(&tmp[i]) == EXIT_FAILURE) {
			std::string error_msg = "Error line [ ~~~ " + *it + " ~~~ ]";
			throw ConfigurationFile::WrongInfo();
			std::cout << *it << std::endl;
		}
	}
}

void	ConfigurationFile::dirServer( void ) {
	
}

void	ConfigurationFile::checkAllDirectives( void ) {

	static int line = 0;
	std::vector< std::string >::iterator	it;
	
	line++;
	// if (*it[0] == '\0' || *str == '#' || *str == '{' || *str == '}')
		// return ;

	const t_dir whichDirective[] = {
		{"server_name ", &ConfigurationFile::dirServer},
		// {"server", ft_server},
		// {"listen ", ft_listen},
		// {"root ", ft_root},
		// {"index ", ft_index},
		// {"get_methods ", ft_get_methods},
		// {"client_max_body_size ", ft_client_max_body_size},
		// {"autoindex ", ft_autoindex},
		// {"location ", ft_location},
		// {"cgi ", ft_cgi},
		// {"auth ", ft_auth},
		// {"return ", ft_return}
		};(void)whichDirective;

	it = this->_file.begin();
	while (it < this->_file.end()) {
		std::string::iterator	i = (*it).begin();
		while (*i == ' ' || *i == '\t')
			*i++;
		std::string ret = this->whichDirective(&(*i));
		if (ret != "") {
			(this->*(whichDirective[0].f))();
			// std::cout << "ret = " << ret << std::endl;
			// return ;
		}
		*it++;
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

std::string	ConfigurationFile::whichDirective( std::string str ) {

	if (str.find("server_name ") != std::string::npos)
		return "server_name";
	else if (str.find("root ") != std::string::npos)
		return "root";
	else if (str.find("listen ") != std::string::npos)
		return "listen";
	else if (str.find("index ") != std::string::npos)
		return "index";
	else if (str.find("get_methods ") != std::string::npos)
		return "get_methods";
	else if (str.find("auth ") != std::string::npos)
		return "auth";
	else if (str.find("client_max_body_size ")  != std::string::npos)
		return "client_max_body_size";
	else if (str.find("autoindex ") != std::string::npos)
		return "autoindex";
	else if (str.find("location ") != std::string::npos)
		return "location";
	else if (str.find("cgi ") != std::string::npos)
		return "cgi";
	else if (str.find("return ") != std::string::npos)
		return "return";
	
	return "";
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
