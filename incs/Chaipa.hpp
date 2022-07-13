/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Chaipa.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/06 14:30:32 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/13 15:05:11 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHAIPA_HPP
# define CHAIPA_HPP

#include "ConfigurationFile.hpp"
#include "Server.hpp"

class Chaipa {
	
public:

	void	checkAll( const char *str ) {
		try {
			this->getConfigurationFile().setNameFile(str);
			this->getConfigurationFile().checkFileName();
			this->getConfigurationFile().setFileVector();
			this->getConfigurationFile().setArgsFile();
			this->getConfigurationFile().checkFileAllTogether();
			// this->getConfigurationFile().checkNothingOut();
			this->getConfigurationFile().checkAllDirectives();
			
		}
		catch (const std::exception &e) {
			std::cout << e.what() << std::endl;
			exit (EXIT_FAILURE);
		}
	}

	ConfigurationFile	&getConfigurationFile( void ) { return this->_config; }

//	CANONICAL FORM

	Chaipa(/* args */);
	~Chaipa();

private:

	ConfigurationFile _config;

};

Chaipa::Chaipa(/* args */)
{
}

Chaipa::~Chaipa()
{
}


#endif