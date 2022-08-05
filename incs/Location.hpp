/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:20:51 by ldermign          #+#    #+#             */
/*   Updated: 2022/08/05 18:32:07 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "webserv.hpp"

class	Location {

public:

//

	int							setLocation( std::vector< std::string >::iterator it );
	int							setMethods( std::vector< std::string >::iterator it );
	int							setReturn( std::vector< std::string >::iterator it );
	int							setRoot( std::vector< std::string >::iterator it );
	int							setIndex( std::vector< std::string >::iterator it );
	int							setAutoindex( std::vector< std::string >::iterator it );
	int							setCgi( std::vector< std::string >::iterator it );
	int							setDownload( std::vector< std::string >::iterator it );

//	ACCESSORS

	Location					&getLocation( void ) { return *this; }

	std::string								getPath( void ) const { return this->_path; }
	std::vector< std::string >				&getIndex( void ) { return this->_index; }
	std::string								getRoot( void ) const { return this->_root; }
	bool									getAutoindex( void ) const { return this->_autoindex; }
	std::vector< std::string >				&getMethods( void ) { return this->_methods; }
	size_t									getReturnCode( void ) const { return this->_returnCode; }
	std::string								getReturnPath( void ) const { return this->_returnPath; }
	std::pair< std::string, std::string >	&getCgi( void ) { return this->_cgi; }
	std::string								getDownload( void ) const { return this->_download; }

//	CANONICAL FORM

	Location( void )
		: _path(""), _index(), _root("./files_config/"), _autoindex(0), _methods(), _returnCode(0),
		_returnPath("./files_config/"), _cgi(), _download("./files_config/") {}
	Location	&operator=( const Location &rhs ) {
		if (this == &rhs)
			return *this;

		this->_path = rhs._path;
		this->_index = rhs._index;
		this->_root = rhs._root;
		this->_autoindex = rhs._autoindex;
		this->_methods = rhs._methods;
		this->_returnCode = rhs._returnCode;
		this->_returnPath = rhs._returnPath;
		this->_cgi = rhs._cgi;
		this->_download = rhs._download;
		
		return *this;
	}
	Location( const Location &src ) {
		*this = src;
	}
	virtual	~Location( void ) {}

private:

	std::string								_path;
	std::vector< std::string >				_index;
	std::string								_root;
	bool									_autoindex;
	std::vector< std::string >				_methods;
	size_t									_returnCode;
	std::string								_returnPath;
	std::pair< std::string, std::string >	_cgi;
	std::string								_download;

};

#endif
