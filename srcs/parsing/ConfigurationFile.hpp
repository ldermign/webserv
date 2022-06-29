/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 15:35:12 by ldermign          #+#    #+#             */
/*   Updated: 2022/06/29 14:06:12 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONFILE_HPP
# define CONFIGURATIONFILE_HPP

#include "webserv.hpp"

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

//	CANONICAL FORM

	ConfigurationFile( void );
	ConfigurationFile	&operator=( const ConfigurationFile &rhs );
	ConfigurationFile( const ConfigurationFile &src );
	virtual	~ConfigurationFile( void );

private:

	std::string	_rootFile;
	int			_port;
	std::string	_serverName;
	// client_body_buffer_size
	
};

#endif