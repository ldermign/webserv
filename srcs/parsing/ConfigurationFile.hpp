/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 15:35:12 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/05 14:50:51 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONFILE_HPP
# define CONFIGURATIONFILE_HPP

#include "webserv.hpp"
#include <vector>
#include <map>

class ConfigurationFile {
	
public:

//	ACCESSORS

	std::string	getRootFile( void );
	int			getPort( void );
	std::string	getServerName( void );

//	EXCEPTION

	class BadFile : public std::exception {
	public:
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mCouldn't open file.\033[0m");
		}	
	};

	class EmptyFile : public std::exception {
	public:
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mFile is empty.\033[0m");
		}	
	};

	class FileIsDir : public std::exception {
	public:
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mArgument is a directory.\033[0m");
		}	
	};

	class BadBracket : public std::exception {
	public:
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mCheck brackets in file.\033[0m");
		}
	};

//	CANONICAL FORM

	ConfigurationFile( void );
	ConfigurationFile	&operator=( const ConfigurationFile &rhs );
	ConfigurationFile( const ConfigurationFile &src );
	virtual	~ConfigurationFile( void );

private:

	std::vector< std::string >	_listen;
	std::vector< std::string >	_serverName;
	// std::map< int >				_clientMaxBodySize; revoir caaa la
	std::vector< std::string >	_methods;
	std::vector< std::string >	_index;
	std::string					_root;
	std::string					_cgi;
	bool						_autoIndex;

	// int			_port;
	
	// la location qui doit avoir une classe dedans;
	// voir comment faire le port
	
};

typedef	struct s_getDirective {

	const char	*directive;
	int			(*f)(char const *str);

}	t_dir;

#endif