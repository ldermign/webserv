/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 15:35:12 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/15 14:29:49 by ldermign         ###   ########.fr       */
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

	// typedef typename std::vector< std::string >::iterator iterator;

public:

//	FUNCTIONS

	void		checkFileName( void );
	void		checkFileAllTogether( void );
	void		setFileVector( void );
	void		setArgsFile( void );
	void		setPos( void ) { this->_pos++; }
	
	void		checkNothingOut( void );	// nein!
	void		checkAllDirectives( void );	// nein!

	int			dirBlockServer( std::string str );
	int			dirBlockLocation( std::string str );

/* check directives */
	int			dirServerName( std::vector< std::string >::iterator it );
	int			dirListen( std::vector< std::string >::iterator it );
	int			dirClientMaxBodySize( std::vector< std::string >::iterator it );
	int			dirErrorPage( std::vector< std::string >::iterator it );
	int			dirLocation( std::vector< std::string >::iterator it );
	int			dirGetMethods( std::vector< std::string >::iterator it );
	int			dirReturn( std::vector< std::string >::iterator it );
	int			dirRoot( std::vector< std::string >::iterator it );
	int			dirIndex( std::vector< std::string >::iterator it );
	int			dirAutoindex( std::vector< std::string >::iterator it );
	int			dirCgi( std::vector< std::string >::iterator it );
	int			dirDownload( std::vector< std::string >::iterator it );
	// 10 11
	

/* utils directiives */
	int			isDirectiveServer( char const *str );
	int			noDirective( std::string str );

//	MUTATORS

	void		setNameFile( char const *name ) { this->_nameFile = name; }
	void		setNbrServer( int nbr ) { this->_nbrServer = nbr; }

//	ACCESSORS

	char const *getNameFile( void ) const { return this->_nameFile; }
	int			getNbrServer( void ) const { return this->_nbrServer; }
	int			getPos( void ) const { return this->_pos; }
	std::vector< std::string >	&getFile( void ) { return this->_file; }

	// void lexerToken( std::string );

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

	class BadInstruction : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mMissing at least one ';'.\033[0m");
		}
	};
	
	class BadEnd : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mNo info after ';'.\033[0m");
		}
	};

	class BadDirectiveAutoindex : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive autoindex.\033[0m");
		}
	};

	class BadDirectiveRoot : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive root.\033[0m");
		}
	};

	class BadDirectiveServerName : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive server_name.\033[0m");
		}
	};

	class BadDirectiveClient : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive client_max_body_size.\033[0m");
		}
	};

	class BadDirectiveIndex : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive index.\033[0m");
		}
	};

	class BadDirectiveMethods : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive get_methods.\033[0m");
		}
	};

	class BadDirectiveListen : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive listen.\033[0m");
		}
	};

	class BadDirectiveReturn : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive return.\033[0m");
		}
	};

	class BadDirectiveErrorPage : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive error_page.\033[0m");
		}
	};

	class BadDirectiveLocation : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive location.\033[0m");
		}
	};

	class BadDirectiveCgi : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive cgi.\033[0m");
		}
	};

	class BadDirectiveDownload : public std::exception {
		virtual const char	*what( void ) const throw() {
			return ("\033[38;5;124mWrong info in directive download.\033[0m");
		}
	};

//	CANONICAL FORM

	ConfigurationFile( void ) : _pos(0) {}
	ConfigurationFile	&operator=( const ConfigurationFile &rhs );
	ConfigurationFile( const ConfigurationFile &src );
	virtual	~ConfigurationFile( void ) {}

private:

	int							_nbrServer;
	char const					*_nameFile;
	std::vector< std::string >	_file;
	std::vector< std::string >	_args;
	std::string					_locationTmp;
	int							_pos;
	
};

typedef	struct s_getDirective {

	const char	*directive;
	void		(ConfigurationFile::*f)( std::string::iterator str );

}	t_dir;

#endif
