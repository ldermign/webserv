/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:20:51 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/19 18:46:17 by ldermign         ###   ########.fr       */
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

	std::string					getPath( void ) { return this->_path; }
	std::vector< std::string >	&getIndex( void ) { return this->_index; }
	std::string					getRoot( void ) { return this->_root; }
	bool						getAutoindex( void ) { return this->_autoindex; }
	std::vector< std::string >	&getMethods( void ) { return this->_methods; }
	size_t						getReturnCode( void ) { return this->_returnCode; }
	std::string					getReturnPath( void ) { return this->_returnPath; }
	std::string					getCgiExtension( void ) { return this->_cgiExtension; }
	std::string					getCgiPath( void ) { return this->_cgiPath; }
	std::string					getDownload( void ) { return this->_download; }

//	CANONICAL FORM

	Location( void )
		: _path(""), _index(), _root("./files_config/"), _autoindex(0), _methods(), _returnCode(0),
		_returnPath("./files_config/"), _cgiExtension(""), _cgiPath("./files_config/"), _download("./files_config/") {}
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
		this->_cgiExtension = rhs._cgiExtension;
		this->_cgiPath = rhs._cgiPath;
		this->_download = rhs._download;
		
		return *this;
	}
	Location( const Location &src ) {
		*this = src;
	}
	virtual	~Location( void ) {}

private:

	std::string					_path;
	std::vector< std::string >	_index;
	std::string					_root;
	bool						_autoindex;
	std::vector< std::string >	_methods;
	size_t						_returnCode;
	std::string					_returnPath;
	std::string					_cgiExtension;
	std::string					_cgiPath;
	std::string					_download;

};

#endif