/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 14:22:32 by ldermign          #+#    #+#             */
/*   Updated: 2022/06/24 10:06:16 by mlebrun          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigurationFile.hpp"

/*
**	CANONICAL FORM
*/

ConfigurationFile::ConfigurationFile( void ) {}

ConfigurationFile	&ConfigurationFile::operator=( const ConfigurationFile &rhs ) {

	if (this != &rhs) {
		
	}
	
	return *this;
}

ConfigurationFile::ConfigurationFile( const ConfigurationFile &src ) {
	
	*this = src;
}

ConfigurationFile::~ConfigurationFile( void ) {}
