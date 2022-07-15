/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/15 15:20:51 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/15 15:40:30 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "webserv.hpp"

class	Location {

public:

private:

	std::string					_path;
	std::vector< std::string >	_index;
	std::string					_root;
	std::string					_download;
	bool						_autoindex;
	// voir pur get_methods
	

};

#endif