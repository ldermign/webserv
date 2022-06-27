/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 13:31:34 by ldermign          #+#    #+#             */
/*   Updated: 2022/06/27 13:32:23 by ldermign         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#define QUEUE_SIZE 100
#define PORT 3455
#define BUFFER_SIZE 10

int	initServ(void);
int	recv_request(int fd);

/*
**	PARSING CONFIG
*/

void	recup_config_file( char const *str );

#endif