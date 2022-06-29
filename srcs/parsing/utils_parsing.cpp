/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_parsing.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/29 09:36:34 by ldermign          #+#    #+#             */
/*   Updated: 2022/06/29 09:47:04 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
// #include "ConfigurationFile.hpp"
#include <string>
// #include <exception>
// #include <sys/stat.h>
// #include <cstdlib>

int	is_directive( std::string const &str ) {

	return (str.find("server", 0) || str.find("listen", 0)
		|| str.find("server_name", 0) || str.find("root", 0)
		|| str.find("location", 0) || str.find("client_body_buffer_size", 0)
		|| str.find("get_methods", 0) || str.find("autoindex", 0)
		|| str.find("index", 0) || str.find("cgi", 0));

}