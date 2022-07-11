/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldermign <ldermign@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 13:31:34 by ldermign          #+#    #+#             */
/*   Updated: 2022/07/11 21:57:29 by jbondri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP
# include <iostream>
# include <netinet/in.h>
# include <unistd.h>
# include <cstring>
# include <poll.h>
# define QUEUE_SIZE 100
# define PORT 3455
# define BUFFER_SIZE 10
# include <sys/select.h>
# include <vector>
# include <algorithm>
# define SEND 100
# define ACCEPT 101
# define RECV 102
# define NONE 99
# define BUFF_SIZE 10000
# include <stdio.h>
# include <netdb.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# define RESET		"\033[0m"
# define BLACK		"\033[30m"      
# define RED     	"\033[31m"      
# define GREEN		"\033[32m"      
# define YELLOW		"\033[33m"      
# define BLUE		"\033[34m"      
# define MAGENTA		"\033[35m"      
# define CYAN		"\033[36m"      
# define WHITE   	"\033[37m"
# define BOLDBLACK   "\033[1m\033[30m"
# define RMLINE		"\033[2K"

int	initServ(void);
int	recv_request(int fd);

/*
**	PARSING CONFIG
*/

void	recup_config_file( char const *str );
int		is_directive( const char *str );

#endif
