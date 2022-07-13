/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 14:22:32 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/13 15:29:01 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <cstdlib>
#include "ConfigurationFile.hpp"

void	ConfigurationFile::checkFileAllTogether( void ) {

	std::vector< std::string >::iterator	it;	// declare an iterator to a vector of strings
	int bracket = 0;
	int server = 0;

	for (it = this->_args.begin() ; it < this->_args.end() ; *it++) {
		if (*it == "{")
			bracket++;
		if (*it == "}")
			bracket--;
		if (*it == "server")
			server++;
		// if (*it == "location") {
		// 	if (*it == "{") { std::string error_msg = "\033[38;5;124mMissing path for location : [ ~ " + static_cast< std::string >(*it) + " ~ ]\033[0m";
		// 		throw std::runtime_error("\033[38;5;124mMissing path for location\033[0m");}
		// 	if (this->dirBlockLocation() == EXIT_FAILURE)
		// 		throw std::runtime_error("\033[38;5;124mCheck info in bloc Location")
		// }
		// if (this->dirBlockServer() == EXIT_FAILURE)
		// 	throw ConfigurationFile::WrongInfo();
	}

	this->setNbrServer(server);
	if (bracket != 0)
		throw ConfigurationFile::BadBracket();
	else if (server == 0)
		throw ConfigurationFile::NbrServer();

	// std::string error_msg = "Error line [ ~~~ " + *it + " ~~~ ]";
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

void	ConfigurationFile::setArgsFile( void ) {
	
	int i = 0, len = 0;
	std::string tmp;
	std::vector< std::string >::iterator	it;
	
	it = this->_file.begin();
	while (it != this->_file.end()) {

		i = 0;
		std::string	tmp = (*it);
		while (tmp[i] != '\0') {
			while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t'))
				i++;
			if (!tmp[i] || tmp[i] == '#')
				break ;
			len = 0;
			while (tmp[i] && tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != '#' && tmp[i] != ';') {
				i++;
				len++;
			}
			if (tmp[i] == ';')
				len++;
			this->_args.push_back((tmp.substr(i - len, len)).c_str());
		}
		*it++;
	}
	for (unsigned long i = 0 ; i < this->_args.size() ; i++)
		std::cout << "[" << this->_args[i] << "]" << std::endl;

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
		if ((this->isDirectiveServer((*it).c_str()) == EXIT_FAILURE && (*it)[0] != '\0' && (*it).find('}') == std::string::npos && bracket == 0 && (*it)[0] != '#')
			|| (server == 0 && this->isDirectiveServer((*it).c_str()) == EXIT_FAILURE && (*it)[0] != '\0' && (*it)[0] != '#')) {
			throw ConfigurationFile::BlockServer();
		}

		std::string tmp = it->c_str();
		int i = 0;
		while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t'))
			i++;

		if (this->isDirectiveServer(&tmp[i]) == EXIT_FAILURE
			&& this->noDirective(&tmp[i]) == EXIT_FAILURE) {
			std::string error_msg = "Error line [ ~~~ " + *it + " ~~~ ]";
			std::cout << error_msg << std::endl;
			throw ConfigurationFile::WrongInfo();
		}
	}
}

void	ConfigurationFile::dirServer( std::string::iterator str ) {(void)str;
	std::cout << "directive Server" << std::endl;


}

void	ConfigurationFile::dirServerName( std::string::iterator str ) {

	while (*str) {
		if (isupper(*str))
			throw ConfigurationFile::BadDirectiveServerName();
		*str++;
	}

}

int	wrongIP( std::string str ) {

	int i = 0, tmp = 0, ret1 = 0, ret2 = 0;
	
	if (str.find_first_not_of("0123456789.") != std::string::npos)
		return EXIT_FAILURE;
	while (str[i]) {
		
		if (str[i] == '.')
			return EXIT_FAILURE;
		tmp = atoi(&(str[i]));
		if (tmp < 0 || tmp > 256)
			return EXIT_FAILURE;
		while (str[i] && str[i] != '.')
			i++;
		if (str[i] == '.') {
			ret2++;
			i++;
		}
		ret1++;
	}
	if (ret1 != 4 || ret2 != 3)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int	wrongPort( std::string str ) {

	if (str.find_first_not_of("0123456789") != std::string::npos
		|| atoi(str.c_str()) > 65535 || atoi(str.c_str()) <= 0)
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

void	ConfigurationFile::dirListen( std::string::iterator str ) {

	int i = 0, len = 0;
	std::string tmp = &(*str);
	std::vector< std::string >	args;

	while (tmp[i]) {
		if (tmp[i] == ';')
			break ;
		while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t' || tmp[i] == ';' || tmp[i] == ':'))
			i++;
		if (tmp[i] == ';' || !tmp[i])
			break ;
		len = 0;
		while (tmp[i] && tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != ';' && tmp[i] != ':') {
			i++;
			len++;
		}
		args.push_back((tmp.substr(i - len, len)).c_str());
	}
	for (unsigned long i = 0 ; i < args.size() ; i++)
		std::cout << "[" << args[i] << "]" << std::endl;

	if (args.size() <= 0 || args.size() > 3 || (args.size() == 3 && args.back() != "default_server"))
		throw ConfigurationFile::BadDirectiveListen();
	else if (args.size() == 1 && args[0] != "default_server"
		&& wrongIP(args[0]) == EXIT_FAILURE
		&& wrongPort(args[0]) == EXIT_FAILURE)
		throw ConfigurationFile::BadDirectiveListen();
	else if (args.size() == 2
		&& ((args[0] != "default_server" && wrongIP(args[0]) == EXIT_FAILURE && wrongPort(args[0]) == EXIT_FAILURE)
			|| (args[0] != "default_server" && wrongIP(args[0]) == EXIT_FAILURE && wrongPort(args[0]) == EXIT_FAILURE)))
		throw ConfigurationFile::BadDirectiveListen();
	else if (args.size() == 3 && (wrongIP(args[0]) == EXIT_FAILURE || wrongPort(args[1]) == EXIT_FAILURE))
		throw ConfigurationFile::BadDirectiveListen();

}

void	ConfigurationFile::dirRoot( std::string::iterator str ) {

	if (*str == '\0' || *str == ';')
		throw ConfigurationFile::BadDirectiveRoot();
	
	std::ifstream tmp;
	std::string	ok = &(*str);
	int i = 0;
	for ( ; ok[i] != ';' ; i++) {}
	tmp.open((ok.substr(0, i)).c_str());
	
	if (tmp.fail())
		throw ConfigurationFile::BadDirectiveRoot();
	
	{
		bool empty = (tmp.get(), tmp.eof());
		if (empty) {
			tmp.close();
			throw ConfigurationFile::BadDirectiveRoot();
		}
	}
	tmp.close();
}

void	ConfigurationFile::dirIndex( std::string::iterator str ) {
	
	if (*str == '\0' || *str == ';')
		throw ConfigurationFile::BadDirectiveIndex();
	
	std::ifstream tmp;
	std::string	ok = &(*str);
	int i = 0;
	for ( ; ok[i] != ';' ; i++) {}
	tmp.open((ok.substr(0, i)).c_str());
	
	if (tmp.fail())
		throw ConfigurationFile::BadDirectiveIndex();
	
	{
		bool empty = (tmp.get(), tmp.eof());
		if (empty) {
			tmp.close();
			throw ConfigurationFile::BadDirectiveIndex();
		}
	}
	tmp.close();
}

void	ConfigurationFile::dirGetMethods( std::string::iterator str ) {

	int i = 0, len = 0;
	std::string tmp = &(*str);
	std::vector< std::string >	args;

	while (tmp[i]) {
		if (tmp[i] == ';')
			break ;
		while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t' || tmp[i] == ';' || tmp[i] == ':'))
			i++;
		if (tmp[i] == ';' || !tmp[i])
			break ;
		len = 0;
		while (tmp[i] && tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != ';' && tmp[i] != ':') {
			i++;
			len++;
		}
		args.push_back((tmp.substr(i - len, len)).c_str());
	}
	// for (unsigned long i = 0; i < args.size(); i++)
	// 	std::cout << "[" << args[i] << ']' << std::endl;

	if (args.size() < 1 || args.size() > 3)
		throw ConfigurationFile::BadDirectiveMethods();
	
	if ((args[0] != "GET" && args[0] != "POST" && args[0] != "DELETE")
		|| (args.size() > 1 && args[1] != "GET" && args[1] != "POST" && args[1] != "DELETE")
		|| (args.size() > 2 && args[2] != "GET" && args[2] != "POST" && args[2] != "DELETE"))
		throw ConfigurationFile::BadDirectiveMethods();

	if (args.size() > 1) {
		if (args[0] == args[1])
			throw ConfigurationFile::BadDirectiveMethods();
		if (args.size() > 2 && (args[1] == args[2] || args[0] == args[2]))
			throw ConfigurationFile::BadDirectiveMethods();
	}
	
}

void	ConfigurationFile::dirClientMaxBodySize( std::string::iterator str ) {

	std::string tmp = static_cast< std::string >(&(*str));
	while (*str) {
		if (!std::isdigit(*str) && *str != ';')
			throw ConfigurationFile::BadDirectiveClient();
		*str++;
	}

}

void	ConfigurationFile::dirAutoindex( std::string::iterator str ) {

	int i = 0, len = 0;
	std::string tmp = &(*str);
	std::vector< std::string >	args;

	while (tmp[i]) {
		if (tmp[i] == ';')
			break ;
		while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t' || tmp[i] == ';' || tmp[i] == ':'))
			i++;
		if (tmp[i] == ';' || !tmp[i])
			break ;
		len = 0;
		while (tmp[i] && tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != ';' && tmp[i] != ':') {
			i++;
			len++;
		}
		args.push_back((tmp.substr(i - len, len)).c_str());
	}
	
	if (args.size() < 1 || args.size() > 2
		|| (args.size() == 2 && args[1] != "on" && args[0] != "off"))
		throw ConfigurationFile::BadDirectiveAutoindex();

}

void	ConfigurationFile::dirLocation( std::string::iterator str ) {(void)str;
	std::cout << "directive Location" << std::endl;
}

void	ConfigurationFile::dirCgi( std::string::iterator str ) {(void)str;
	std::cout << "directive Cgi" << std::endl; //(premier extension, 2eme chemin)

	
}

void	ConfigurationFile::dirReturn( std::string::iterator str ) {

	int i = 0, len = 0;
	std::string tmp = &(*str);
	std::vector< std::string >	args;

	while (tmp[i]) {
		if (tmp[i] == ';')
			break ;
		while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t' || tmp[i] == ';' || tmp[i] == ':'))
			i++;
		if (tmp[i] == ';' || !tmp[i])
			break ;
		len = 0;
		while (tmp[i] && tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != ';' && tmp[i] != ':') {
			i++;
			len++;
		}
		args.push_back((tmp.substr(i - len, len)).c_str());
	}
	
	if (args.size() < 3)
		throw ConfigurationFile::BadDirectiveReturn();
	
	std::ifstream tmp2;
	tmp2.open((args[args.size() - 1]).c_str());
	if (tmp2.fail())
		throw ConfigurationFile::BadDirectiveIndex();
	
	{
		bool empty = (tmp2.get(), tmp2.eof());
		if (empty) {
			tmp2.close();
			throw ConfigurationFile::BadDirectiveIndex();
		}
	}
	tmp2.close();
	
}

void	ConfigurationFile::checkAllDirectives( void ) {
return ;
	// std::vector< std::string >::iterator	it;

	// it = this->_file.begin();
	// while (it < this->_file.end()) {

	// 	if (this->dirBlockServer() == EXIT_SUCCESS) {
			
	// 		(this->*(whichDirective[j].f))(i);
	// 		if (j != 7 && static_cast< std::string >(&(*i)).find(';') == std::string::npos)
	// 			throw ConfigurationFile::BadInstruction();
	// 		for ( ; *i && *i != ';' ; *i++) {}
	// 		if (*i != '\0')
	// 			*i++;
	// 		if (*i && *i != '\0')
	// 			throw ConfigurationFile::BadEnd();
	// 	}
	// 	else
	// 		throw ConfigurationFile::WrongInfo();
	// 	*it++;
	// }

}

/*	Check directives */

int	ConfigurationFile::noDirective( std::string str ) {

	if (str.find("server_name ") == std::string::npos
		// && str.find("root ") == std::string::npos
		&& str.find("listen ") == std::string::npos
		// && str.find("index ") == std::string::npos
		// && str.find("get_methods ") == std::string::npos
		&& str.find("client_max_body_size ")  == std::string::npos
		// && str.find("autoindex ") == std::string::npos
		&& str.find("location ") == std::string::npos
		// && str.find("cgi ") == std::string::npos
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

int	ConfigurationFile::dirBlockServer( std::string str ) {

	if (str != "listen" && str != "server_name" && str != "error_page" && str != "client_max_body_size")
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int	ConfigurationFile::dirBlockLocation( std::string str ) {

	if (str != "get_methods" && str != "return" && str != "root" && str != "index" && str != "autoindex" && str != "cgi" && str != "download")
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
