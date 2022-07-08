/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 15:35:12 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/08 14:23:01 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONFILE_HPP
# define CONFIGURATIONFILE_HPP

#include "webserv.hpp"
#include <vector>
#include <fstream>
#include <exception>
#include <sys/stat.h>
#include <cstdlib>
#include <sstream>

class ConfigurationFile {
	
public:

//	FUNCTIONS

	void		checkFileName( void );
	void		checkFileAllTogether( void );
	void		setFileVector( void );
	void		checkNothingOut( void );
	void		checkAllDirectives( void );

/* check directives */
	void		dirServer( void );

/* utils directiives */
	int			isDirectiveServer( char const *str );
	int			noDirective( std::string str );
	std::string	whichDirective( std::string str );

//	MUTATORS

	void		setNameFile( char const *name ) { this->_nameFile = name; }
	void		setNbrServer( int nbr ) { this->_nbrServer = nbr; }

//	ACCESSORS

	char const *getNameFile( void ) const { return this->_nameFile; }
	int			getNbrServer( void ) const { return this->_nbrServer; }

//	EXCEPTION

	class BadFile : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mCouldn't open file.\033[0m");
		}
	};

	class EmptyFile : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mFile is empty.\033[0m");
		}
	};

	class FileIsDir : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mArgument is a directory.\033[0m");
		}
	};

	class BadBracket : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mCheck brackets in file.\033[0m");
		}
	};

	class BlockServer : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mSomething's out of server block.\033[0m");
		}
	};

	class WrongInfo : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in file.\033[0m");
		}
	};

	class NbrServer : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mMissing at least one server bloc.\033[0m");
		}
	};

//	CANONICAL FORM

	ConfigurationFile( void ) {}
	ConfigurationFile	&operator=( const ConfigurationFile &rhs );
	ConfigurationFile( const ConfigurationFile &src );
	virtual	~ConfigurationFile( void ) {}

private:

	int							_nbrServer;
	char const					*_nameFile;
	std::vector< std::string >	_file;
	
};

typedef	struct s_getDirective {

	const char	*directive;
	void		(ConfigurationFile::*f)( void );

}	t_dir;

#endif