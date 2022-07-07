#pragma once
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#define QUEUE_SIZE 100
#define PORT 3245
#define BUFFER_SIZE 10
#include <sys/select.h>
#include <vector>
#define SEND 100
#define ACCEPT 101
#define RECV 102
#define NONE 99
#define BUFF_SIZE 10000
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
//color part
#define RESET		"\033[0m"
#define BLACK		"\033[30m"      /* Black */
#define RED     	"\033[31m"      /* Red */
#define GREEN		"\033[32m"      /* Green */
#define YELLOW		"\033[33m"      /* Yellow */
#define BLUE		"\033[34m"      /* Blue */
#define MAGENTA		"\033[35m"      /* Magenta */
#define CYAN		"\033[36m"      /* Cyan */
#define WHITE   	"\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define RMLINE		"\033[2K"
