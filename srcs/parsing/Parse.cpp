/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parse.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 14:22:32 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/15 14:53:07 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cstdio>
#include <cstdlib>
#include "Parse.hpp"


void	Parse::checkFileAllTogether( void ) {

	std::vector< std::string >::iterator	it;	// declare an iterator to a vector of strings
	int bracket = 0;
	int server = 0;

	for (it = this->_args.begin() ; it < this->_args.end() ; *it++) {
		if (*it == "{")
			bracket++;
		if (*it == "}")
			bracket--;
		if (*it == "server") {
			server++;
			*it++;
			if (*it != "{")
				throw std::runtime_error("Wrong info just after block server.");
			bracket++;
		}
		if (*it == ";") {
			*it++;
			if (*it == "}")
				bracket--;
			if (this->dirBlockServer(*it) == EXIT_FAILURE && this->dirBlockLocation(*it) == EXIT_FAILURE
				&& *it != "}")
				{
					std::cout << "de quoi la " << *it << std::endl;
					throw Parse::WrongInfo();
				}
		}

	}

	this->setNbrServer(server);
	if (bracket != 0)
		throw Parse::BadBracket();
	else if (server == 0)
		throw Parse::NbrServer();

	// std::string error_msg = "Error line [ ~~~ " + *it + " ~~~ ]";
}

void	Parse::checkFileName( void ) {

	std::ifstream tmp;
	tmp.open(this->getNameFile());

	if (tmp.fail())
		throw Parse::BadFile();
	{
		bool empty = (tmp.get(), tmp.eof());
		if (empty) {
			tmp.close();
			throw Parse::EmptyFile();
		}
	}
	{
		struct stat path_stat;
    	stat(this->getNameFile(), &path_stat);
    	if (!S_ISREG(path_stat.st_mode))
			throw Parse::FileIsDir();
	}
	tmp.close();
}

int	Parse::isDirectiveServer( char const *str ) {

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

void	Parse::setFileVector( void ) {
	
	std::string line;
	std::ifstream tmp(this->getNameFile());

	do {
		std::getline(tmp, line);
		this->_file.push_back(line);
	} while (!tmp.eof());
	tmp.close();
}

void	Parse::setArgsFile( void ) {
	
	int i = 0, len = 0;
	std::string tmp, tmp2;
	std::vector< std::string >::iterator	it;
	
	it = this->_file.begin();
	while (it != this->_file.end()) {

		i = 0;
		std::string	tmp = (*it);
		while (tmp[i] != '\0') {

			if (tmp[i] == ':')
				i++;
			while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t'))
				i++;
			if (!tmp[i] || tmp[i] == '#')
				break ;
			len = 0;
			while (tmp[i] && tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != '#' && tmp[i] != ';' && tmp[i] != ':' && tmp[i] != '{' && tmp[i] != '}') { // && tmp[i] != '{' && tmp[i] != '}'
				i++;
				len++;
			}
			tmp2 = tmp.substr(i - len, len);
			if (tmp2 != "" && tmp2 != ":")
				this->_args.push_back(tmp2.c_str());
			if (tmp[i] == ';' || tmp[i] == '}' || tmp[i] == '{') {
				if (tmp[i] == ';')
					this->_args.push_back(";");
				else if (tmp[i] == '}')
					this->_args.push_back("}");
				else
					this->_args.push_back("{");
				i++;
			}
		}
		*it++;
	}
	// for (unsigned long i = 0 ; i < this->_args.size() ; i++)
	// 	std::cout << "[" << this->_args[i] << "]" << std::endl;

}

void	Parse::checkNothingOut( void ) {

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
			throw Parse::BlockServer();
		}

		std::string tmp = it->c_str();
		int i = 0;
		while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t'))
			i++;

		if (this->isDirectiveServer(&tmp[i]) == EXIT_FAILURE
			&& this->noDirective(&tmp[i]) == EXIT_FAILURE) {
			std::string error_msg = "Error line [ ~~~ " + *it + " ~~~ ]";
			std::cout << error_msg << std::endl;
			throw Parse::WrongInfo();
		}
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

int	Parse::dirServerName( std::vector< std::string >::iterator it ) {

	std::string tmp = it->c_str();
	int ret = 1;

	*it++;
	while (*it != ";") {

		for (int i = 0 ; tmp[i] ; i++) {
			if (isupper(tmp[i]))
				throw Parse::BadDirectiveServerName();
		}
		*it++;
		ret++;
	}

	return ret + 1;
}

int	Parse::dirListen( std::vector< std::string >::iterator it ) {

	int i = 0, len = 0;
	std::vector< std::string >	args;
	
	int ret = 0;
	while (it[ret] != ";")
		ret++;

	*it++;
	std::string tmp = it->c_str(), tmp2;
	while (*it != ";") {
	
		while (tmp[i] && (tmp[i] == ' ' || tmp[i] == '\t' || tmp[i] == ';' || tmp[i] == ':'))
			i++;
		if (tmp[i] == ';' || !tmp[i])
			break ;
		len = 0;
		while (tmp[i] && tmp[i] != ' ' && tmp[i] != '\t' && tmp[i] != ';' && tmp[i] != ':') {
			i++;
			len++;
		}
		tmp2 = tmp.substr(i - len, len);
		if (tmp2 != ":" && tmp2 != ":")
			args.push_back(tmp2.c_str());
	}

	// for (unsigned int i = 0 ; i < args.size() ; i++)
	// 	std::cout << args[i] << std::endl;

	if (args.size() <= 0 || args.size() > 3 || (args.size() == 3 && args.back() != "default_server"))
	{
		// std::cout << args.size() << "\n";
		throw Parse::BadDirectiveListen();
	}
	else if (args.size() == 1 && args[0] != "default_server"
		&& wrongIP(args[0]) == EXIT_FAILURE
		&& wrongPort(args[0]) == EXIT_FAILURE)
	{
		std::cout << "1\n";	
		throw Parse::BadDirectiveListen();
	}
	else if (args.size() == 2
		&& ((args[0] != "default_server" && wrongIP(args[0]) == EXIT_FAILURE && wrongPort(args[0]) == EXIT_FAILURE)
			|| (args[0] != "default_server" && wrongIP(args[0]) == EXIT_FAILURE && wrongPort(args[0]) == EXIT_FAILURE)))
	{
		std::cout << "2\n";
		throw Parse::BadDirectiveListen();
	}
	else if (args.size() == 3 && (wrongIP(args[0]) == EXIT_FAILURE || wrongPort(args[1]) == EXIT_FAILURE))
	{
		std::cout << "3\n";
		throw Parse::BadDirectiveListen();
	}

	return ret + 1;
}

int	Parse::dirClientMaxBodySize( std::vector< std::string >::iterator it ) {

	it++;
	std::string tmp = it->c_str();
	int i = 0;
	while (tmp[i]) {
		if (!std::isdigit(tmp[i]))
			throw Parse::BadDirectiveClient();
		i++;
	}

	return 3;
}

int	Parse::dirErrorPage( std::vector< std::string >::iterator it ) {

	int ret = 0, i = 0;
	std::string file;
	
	*it++;
	while (it[ret] != ";")
		ret++;

	if (ret < 2)
	{
		// std::cout << "pas assez arg" << std::endl;
		throw Parse::BadDirectiveErrorPage();
	}
	
	int j = 0;
	while (j < ret - 1) {
		
		std::string file = it->c_str();
		i = 0;
		while (file[i]) {
			if (!std::isdigit(file[i]))
					throw Parse::BadDirectiveErrorPage();
			i++;
		}
		j++;
		// if (file.substr())
		//checker ici que c'est les bons chiffres
		*it++;
	}

	std::ifstream tmp;
	std::string str = it->c_str();
	tmp.open(str.c_str());
	if (tmp.fail()) {
		std::cout << str << std::endl;
		throw Parse::BadDirectiveErrorPage();}
	
	{
		bool empty = (tmp.get(), tmp.eof());
		if (empty) {
			tmp.close();
			// std::cout << str << std::endl;
			throw Parse::BadDirectiveErrorPage();
		}
	}
	tmp.close();
	
	return ret + 2;
}

int	Parse::dirGetMethods( std::vector< std::string >::iterator it ) {

	int ret = 1, len = 0;
	std::string	one, two, three;
	
	*it++;
	while (it[len] != ";")
		len++;
	if (len < 1 || len > 3)
		throw Parse::BadDirectiveMethods();
	one = *it;
	*it++;
	ret++;
	if (*it != "}" && *it != ";") {
		two = *it;
		*it++;
		ret++;
	}
	if (*it != "}" && *it != ";") {
		three = *it;
		*it++;
		ret++;
	}

	
	// std::cout << len << "\t\t\t\tone = " << one << " two = [" << two << "] three = " << three << std::endl;
	
	if ((one != "GET" && one != "POST" && one != "DELETE")
		|| (len > 1 && two != "GET" && two != "POST" && two != "DELETE")
		|| (len > 2 && three != "GET" && three != "POST" && three != "DELETE"))
	{
		std::cout << "1\n";
		throw Parse::BadDirectiveMethods();
	}

	if (len > 1) {
		if (one == two)
			throw Parse::BadDirectiveMethods();
		if (len > 2 && (two == three || one == three))
			throw Parse::BadDirectiveMethods();
	}
	
	return ret + 1;
}

int	Parse::dirReturn( std::vector< std::string >::iterator it ) {

	int ret = 0;
	std::string file, str;

	*it++;
	while (it[ret] != ";") {
		if (ret == 2)
			throw std::runtime_error("Missing one ; at directive return.");
		ret++;
	}
	
		// if (file.substr())
		//checker ici que c'est les bons chiffres pour le code

	*it++;

	std::ifstream tmp;
	str = this->_locationTmp + it->c_str();
	tmp.open(str.c_str());
	if (tmp.fail())
	{
		// std::cout << str << std::endl;
		throw Parse::BadDirectiveReturn();
	}

	{
		bool empty = (tmp.get(), tmp.eof());
		if (empty) {
			tmp.close();
			throw Parse::BadDirectiveReturn();
		}
	}
	tmp.close();
	
	return 4;
}

int	Parse::dirRoot( std::vector< std::string >::iterator it ) {

	int ret = 0;
	
	*it++;
	while (it[ret] != ";") {
		if (ret == 1)
			throw std::runtime_error("Missing one ; at directive root.");
		ret++;
	}
	if (ret == 0)
		throw Parse::BadDirectiveRoot();

	{
		struct stat buffer;
		if (stat(it->c_str(), &buffer) != 0)
			throw std::runtime_error("Wrong directory in directive root.");
	}

	return 3;
}


int	Parse::dirIndex( std::vector< std::string >::iterator it ) {
	
	int ret = 1;
	std::string tmp;

	*it++;
	while (*it != ";") {

		{
			tmp = this->_locationTmp + it->c_str();
			struct stat buffer;
			if (stat(&(tmp[0]), &buffer) != 0)
				throw std::runtime_error("Wrong directory in directive index.");
		}
		*it++;
		ret++;
	}

	return ret + 1;
}


int	Parse::dirAutoindex( std::vector< std::string >::iterator it ) {

	*it++;

	if (*it != "on" && *it != "off")
		throw Parse::BadDirectiveAutoindex();

	return 3;

}


int	Parse::dirCgi( std::vector< std::string >::iterator it ) {

	int ret = 0;

	*it++;
	while (it[ret] != ";") {
		if (ret == 2)
			throw std::runtime_error("Missing one ; at directive cgi.");
		ret++;
	}
	if (ret == 1)
		throw Parse::BadDirectiveCgi();
	
	{
		std::string tmp = it->c_str();
		if (tmp[0] != '.')
			throw Parse::BadDirectiveCgi();
	}

	*it++;
	std::ifstream tmp;
	std::string str = this->_locationTmp + it->c_str();
	tmp.open(str.c_str());
	if (tmp.fail())
		throw Parse::BadDirectiveCgi();
	
	{
		bool empty = (tmp.get(), tmp.eof());
		if (empty) {
			tmp.close();
			throw Parse::BadDirectiveCgi();
		}
	}
	tmp.close();

	return 4;
}

int	Parse::dirDownload( std::vector< std::string >::iterator it ) {

	int ret = 0;

	*it++;
	while (it[ret] != ";") {
		if (ret == 1)
			throw std::runtime_error("Missing one ; at directive download.");
		ret++;
	}
	if (ret == 0)
		throw Parse::BadDirectiveDownload();

	{
		struct stat buffer;
		if (stat(it->c_str(), &buffer) != 0)
			throw std::runtime_error("Wrong directory in directive download.");
	}

	return 3;

}

int	Parse::dirLocation( std::vector< std::string >::iterator it, std::vector< std::string >::iterator last ) {

	int ret = 3, len = 0;

	*it++;
	if (*it == "{")
		throw std::runtime_error("Missing path in directive location.");
	{
		struct stat buffer;
		if (stat(it->c_str(), &buffer) != 0)
    		throw std::runtime_error("Wrong directory in directive location.");
	}

	this->_locationTmp = *it;
	*it++;
	if (*it != "{")
		throw Parse::BadDirectiveLocation();
	*it++;
	// std::cout << *it << std::endl;

	while (it != last && *it != "}") {

		// std::cout << *it << std::endl;
		if (*it == "get_methods")
			len = this->dirGetMethods(it);
		else if (*it == "return")
			len = this->dirReturn(it);
		else if (*it == "root")
			len = this->dirRoot(it);
		else if (*it == "index")
			len = this->dirIndex(it);
		else if (*it == "autoindex")
			len = this->dirAutoindex(it);
		else if (*it == "cgi")
			len = this->dirCgi(it);
		else if (*it == "download")
			len = this->dirDownload(it);
		else
			throw Parse::BadDirectiveLocation();
		ret += len;
		for (int i = 0 ; it != last && i < len ; i++)
			*it++;
	}
	
	// std::cout << "ici " << *it << std::endl;
	return ret + 1;
}

void	Parse::checkAllDirectives( void ) {

	int	ret = 0;
	std::vector< std::string >::iterator	it;

	it = this->_args.begin();
	while (it < this->_args.end()) {

		if (*it == "server") {
			
			*it++;
			*it++;
			while (it < this->_args.end() && *it != "server") {

				// std::cout << *it << std::endl;
				if (*it == "server_name")
					ret = this->dirServerName(it);
				else if (*it == "listen")
					ret = this->dirListen(it);
				else if (*it == "client_max_body_size")
					ret = this->dirClientMaxBodySize(it);
				else if (*it == "error_page")
					ret = this->dirErrorPage(it);
				else if (*it == "location")
					ret = this->dirLocation(it, this->_args.end());
				for (int i = 0 ; it < this->_args.end() && i < ret ; i++)
					*it++;
				if (it < this->_args.end() && (*it == "{" || *it == "}")) {
					while (it < this->_args.end() && (*it == "{" || *it == "}"))
						*it++;
				}
			}
			if (it < this->_args.end() && *it != "{" && *it != "}" && *it != "server")
			{
				// std::cout << "rola " << *it << std::endl;
				throw Parse::WrongInfo();
			}
		}
		// if (it < this->_args.end() && (*it == "{" || *it == "}")) {
		// 	while (it < this->_args.end() && (*it == "{" || *it == "}"))
		// 		*it++;
		// }
	}

}

/*	Check directives */

int	Parse::noDirective( std::string str ) {

	if (str.find("server_name ") == std::string::npos
		&& str.find("listen ") == std::string::npos
		&& str.find("client_max_body_size ")  == std::string::npos
		&& str.find("error_page ") == std::string::npos
		&& str.find("location ") == std::string::npos
		&& str.find(" {") == std::string::npos 
		&& str.find(" }") == std::string::npos
		&& str[0] != '{'
		&& str[0] != '}'
		&& str[0] != '#'
		&& str[0] != '\0')
		return EXIT_FAILURE;
	
	return EXIT_SUCCESS;
		
}


int	Parse::dirBlockServer( std::string str ) {

	if (str != "listen" && str != "server_name" && str != "error_page"
		&& str != "client_max_body_size" && str != "location")
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

int	Parse::dirBlockLocation( std::string str ) {

	if (str != "get_methods" && str != "return" && str != "root" && str != "index" && str != "autoindex" && str != "cgi" && str != "download")
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

/*
**	CANONICAL FORM
*/

Parse	&Parse::operator=( const Parse &rhs ) {

	if (this != &rhs) {
		
	}
	
	return *this;
}

Parse::Parse( const Parse &src ) {
	
	*this = src;
}
