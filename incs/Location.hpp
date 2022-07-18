/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:20:51 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/18 18:05:54 by ldermign         ###   ########.fr       */
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

//	ACCESSORS

	std::string					getPath( void ) { return this->_path; }
	std::vector< std::string >	&getIndex( void ) { return this->_index; }
	std::string					getRoot( void ) { return this->_root; }
	bool						getAutoindex( void ) { return this->_autoindex; }
	//	methodes moncul
	size_t						getCodeReturn( void ) { return this->_codeReturn; }
	std::string					getCodePath( void ) { return this->_codePath; }
	std::string					getCgiExtension( void ) { return this->_cgiExtension; }
	std::string					getCgiPath( void ) { return this->_cgiPath; }
	std::string					getDownload( void ) { return this->_download; }

//	CANONICAL FORM

	Location( void ) {}
	Location	&operator=( const Location &rhs );
	Location( const Location &src );
	virtual	~Location( void ) {}

private:

	std::string					_path;
	std::vector< std::string >	_index;
	std::string					_root;
	bool						_autoindex;
	// voir pour les methodes de merde
	size_t						_codeReturn;
	std::string					_codePath;
	std::string					_cgiExtension;
	std::string					_cgiPath;
	std::string					_download;

};

#endif