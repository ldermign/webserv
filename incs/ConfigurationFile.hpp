/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigurationFile.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 15:35:12 by ldermign          #+#    #+#             */
/*   Updated: 2022/06/21 09:17:49 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGURATIONFILE_HPP
# define CONFIGURATIONFILE_HPP

class ConfigurationFile {
	
public:

//	ACCESSORS


//	CANONICAL FORM

	ConfigurationFile( void );
	ConfigurationFile	&operator=( const ConfigurationFile &rhs );
	ConfigurationFile( const ConfigurationFile &src );
	virtual	~ConfigurationFile( void );


private:


	
};

#endif